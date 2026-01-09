#define _GNU_SOURCE

#include <lcthw/dbg.h>
#include <lcthw/ringbuffer_posix.h>

// 获取系统页大小
size_t get_page_size() {
  static size_t page_size = 0;
  if (page_size == 0) {
    page_size = sysconf(_SC_PAGESIZE);
  }
  return page_size;
}

// 对齐到页大小
static size_t align_to_page_size(size_t size) {
  size_t page_size = get_page_size();
  return (size + page_size - 1) & ~(page_size - 1);
}

// 创建环形缓冲区
RingBuffer *RingBuffer_create_posix(int length) {
  srand((unsigned int)time(NULL));

  RingBuffer *buffer = malloc(sizeof(RingBuffer));
  if (!buffer)
    return NULL;

  memset(buffer, 0, sizeof(RingBuffer));

  // 1. 确保长度是页大小的倍数
  buffer->size = align_to_page_size(length);

  // 2. get fd
  char shm_name[100] = "/ringbuffer_XXXXXX";
  for (int i = 17; i < 23; i++) {
    shm_name[i] = 'A' + rand() % 26;
  }
  shm_name[23] = '\0';
  int fd = shm_open(shm_name, O_CREAT | O_RDWR | O_EXCL, 0600);
  
  if (fd < 0) {
    buffer->use_shm = 0;
    buffer->shm_name = NULL;

    char memfd_name[100] = "ringbuffer";
    fd = memfd_create(memfd_name, MFD_CLOEXEC);

    if (fd < 0) {
      char tmp_name[] = "/tmp/ringbuffer_XXXXXX";
      fd = mkstemp(tmp_name);
      unlink(tmp_name);
      printf("== Using mkstemp to get fd for mmap.\n");

    } else {
      printf("== Using memfd_create to get fd for mmap.\n");
    }
  } else {
    printf("== Using shm_open to get fd for mmap.\n");
    buffer->use_shm = 1;
    buffer->shm_name = malloc(sizeof(shm_name));
    memcpy(buffer->shm_name, shm_name, sizeof(shm_name));
  }

  if (fd < 0) {
    printf("Cannot get a fd for mmap.\n");
    free(buffer);
    return NULL;
  }

  buffer->fd = fd;

  // 扩展文件到所需大小
  if (ftruncate(buffer->fd, buffer->size) < 0) {
    close(buffer->fd);
    free(buffer);
    return NULL;
  }

  // 3. 先预留足够的连续地址空间
  size_t total_size = 2 * buffer->size;
  void *reserved_addr = mmap(NULL, total_size,
                             PROT_NONE, // 无权限，只预留地址
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  printf("reserved address: %p\n", reserved_addr);

  // 4. 解除预留（但不释放地址空间）
  munmap(reserved_addr, total_size);

  // 5. 在预留的地址上映射第一个区域
  buffer->virt_buf1 = mmap(reserved_addr, buffer->size, PROT_READ | PROT_WRITE,
                           MAP_SHARED, buffer->fd, 0);

  if (buffer->virt_buf1 == MAP_FAILED) {
    perror("First mmap failed");
    close(buffer->fd);
    free(buffer);
    return NULL;
  }

  // 6. 在紧接的位置映射第二个区域
  void *desired_addr = (char *)buffer->virt_buf1 + buffer->size;
  buffer->virt_buf2 = mmap(desired_addr, buffer->size, PROT_READ | PROT_WRITE,
                           MAP_FIXED, buffer->fd, 0);

  // 如果指定地址不可用，让内核选择
  if (buffer->virt_buf2 == MAP_FAILED) {
    buffer->virt_buf2 = mmap(NULL, buffer->size, PROT_READ | PROT_WRITE,
                             MAP_SHARED, buffer->fd, 0);
  }

  if (buffer->virt_buf2 == MAP_FAILED) {
    perror("Second mmap failed");
    munmap(buffer->virt_buf1, buffer->size);
    close(buffer->fd);
    free(buffer);
    return NULL;
  }

  // 7. 初始化指针
  buffer->read_ptr = (char *)buffer->virt_buf1;
  buffer->write_ptr = (char *)buffer->virt_buf1;

  close(buffer->fd);
  return buffer;
}

// 调整指针到第一个映射区域
static void adjust_pointers_posix(RingBuffer *buffer) {
  char *buf_start = (char *)buffer->virt_buf1;
  char *buf_end = buf_start + buffer->size;

  // 检查指针是否在第一个映射区域内
  if (buffer->read_ptr >= buf_end) {
    // 计算在第一个区域内的偏移
    size_t offset = buffer->read_ptr - buf_start;
    buffer->read_ptr = buf_start + (offset % buffer->size);
  }

  if (buffer->write_ptr >= buf_end) {
    size_t offset = buffer->write_ptr - buf_start;
    buffer->write_ptr = buf_start + (offset % buffer->size);
  }
}

// 获取可读数据量
int RingBuffer_available_data_posix(RingBuffer *buffer) {
  adjust_pointers_posix(buffer);

  if (buffer->write_ptr >= buffer->read_ptr) {
    return buffer->write_ptr - buffer->read_ptr;
  } else {
    return (buffer->size - (buffer->read_ptr - (char *)buffer->virt_buf1)) +
           (buffer->write_ptr - (char *)buffer->virt_buf1);
  }
}

// 获取可用空间
int RingBuffer_available_space_posix(RingBuffer *buffer) {
  adjust_pointers_posix(buffer);
  return buffer->size - RingBuffer_available_data_posix(buffer);
}

static void continuous_read(RingBuffer *buffer, char *target, int amount) {
  // 由于双映射，我们可以直接从当前位置读取，即使跨越边界
  char *current = buffer->read_ptr;

  char *max_read = (char *)buffer->virt_buf1 + buffer->size;
  if (current + amount > max_read) {
    size_t first_part = max_read - current;
    memcpy(target, current, first_part);
    memcpy(target + first_part, (char *)buffer->virt_buf1, amount - first_part);
  } else {
    memcpy(target, current, amount);
  }

  buffer->read_ptr += amount;
}

int RingBuffer_read_posix(RingBuffer *buffer, char *target, int amount) {
  adjust_pointers_posix(buffer);

  if (amount == 0)
    return 0;

  int available_data = RingBuffer_available_data_posix(buffer);
  if (amount > available_data) {
    printf("Not enough in the buffer: has %d, needs %d, still read\n",
           available_data, amount);
  }

  continuous_read(buffer, target, amount);
  adjust_pointers_posix(buffer);

  return amount;
}

static void continuous_write(RingBuffer *buffer, char *data, int length) {
  char *current = buffer->write_ptr;
  char *max_write = (char *)buffer->virt_buf1 + buffer->size;

  if (current + length > max_write) {
    // 分两次拷贝以确保安全
    size_t first_part = max_write - current;
    memcpy(current, data, first_part);
    memcpy((char *)buffer->virt_buf1, data + first_part, length - first_part);
  } else {
    memcpy(current, data, length);
  }

  buffer->write_ptr += length;
}

// 写入数据
int RingBuffer_write_posix(RingBuffer *buffer, char *data, int length) {
  adjust_pointers_posix(buffer);

  if (length == 0)
    return 0;

  int available_space = RingBuffer_available_space_posix(buffer);
  if (length > available_space) {
    printf("Not enough space in the buffer: %d request, %d available, so cover "
           "the old area.\n",
           length, available_space);
  }

  continuous_write(buffer, data, length);
  adjust_pointers_posix(buffer);

  return length;
}

// 清理资源
void RingBuffer_destroy_posix(RingBuffer *buffer) {
  if (!buffer) {
    return;
  }

  // 解除映射
  if (buffer->virt_buf1 != MAP_FAILED) {
    munmap(buffer->virt_buf1, buffer->size);
  }

  // 第二个映射区域需要单独解除映射
  if (buffer->virt_buf2 != MAP_FAILED &&
      buffer->virt_buf2 != buffer->virt_buf1) {
    munmap(buffer->virt_buf2, buffer->size);
  }

  if (buffer->use_shm) {
    assert(buffer->shm_name != NULL);
    shm_unlink(buffer->shm_name);
    printf("== Using shm_unlink for /dev/shm/%s.\n", buffer->shm_name);
    free(buffer->shm_name);
  }

  free(buffer);
  printf("Destroy ringbuffer over.\n");
}

// 获取内部信息（用于调试）
void RingBuffer_debug_posix(RingBuffer *buffer) {
  printf("========\n");
  if (!buffer) {
    printf("Buffer is NULL, now return.\n");
    return;
  }

  printf("RingBuffer Debug Info:\n");
  printf("  Size: %zu bytes\n", buffer->size);

  printf("  Virt_buf1: %p\n", buffer->virt_buf1);
  printf("  Virt_buf2: %p (offset: %+ld)\n", buffer->virt_buf2,
         (char *)buffer->virt_buf2 - (char *)buffer->virt_buf1);
  printf("  Write ptr: %p (offset: %+ld)\n", buffer->write_ptr,
         buffer->write_ptr - (char *)buffer->virt_buf1);
  printf("  Read  ptr: %p (offset: %+ld)\n", buffer->read_ptr,
         buffer->read_ptr - (char *)buffer->virt_buf1);

  printf("  Available data : %d bytes\n",
         RingBuffer_available_data_posix(buffer));
  printf("  Available space: %d bytes\n",
         RingBuffer_available_space_posix(buffer));

  // 简单的双映射验证
  if ((char *)buffer->virt_buf2 > (char *)buffer->virt_buf1) {
    printf("  Verification: Second mapping is after first\n");
  }

  // 验证双映射是否工作
  char *test_addr = (char *)buffer->virt_buf1 + buffer->size - 1;
  printf("  Verification:\n");
  printf("    Addr at end   of region1: %p\n", test_addr);
  printf("    Addr at start of region2: %p\n", buffer->virt_buf2);
  // printf("    They should point to same physical page\n");
}
