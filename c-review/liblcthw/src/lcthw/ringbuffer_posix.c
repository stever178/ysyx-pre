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
  char shm_name[100] = "ringbuffer_XXXXXX";
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
      log_info("== Using mkstemp to get fd for mmap.");

    } else {
      log_info("== Using memfd_create to get fd for mmap.");
    }
  } else {
    log_info("== Using shm_open to get fd for mmap.");
    buffer->use_shm = 1;
    buffer->shm_name = malloc(sizeof(shm_name));
    memcpy(buffer->shm_name, shm_name, sizeof(shm_name));
  }

  if (fd < 0) {
    log_err("Cannot get a fd for mmap.");
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
  log_info("Reserved address: %p .", reserved_addr);

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
    buffer->is_continuous = false;
    buffer->virt_buf2 = mmap(NULL, buffer->size, PROT_READ | PROT_WRITE,
                             MAP_SHARED, buffer->fd, 0);
  } else {
    assert(buffer->virt_buf2 == desired_addr);
    buffer->is_continuous = true;
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
  if (buffer->write_ptr == buffer->read_ptr) {
    buffer->write_ptr = buffer->read_ptr = buf_start;
    return;
  }

  size_t read_offset = buffer->read_ptr + buffer->size - buf_start;
  buffer->read_ptr = buf_start + (read_offset % buffer->size);

  size_t write_offset = buffer->write_ptr + buffer->size - buf_start;
  buffer->write_ptr = buf_start + (write_offset % buffer->size);
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
  return buffer->size - RingBuffer_available_data_posix(buffer);
}

static int continuous_write(RingBuffer *buffer, char *data, int length) {
  char *current = buffer->write_ptr;

  if (buffer->is_continuous) {
    memcpy(current, data, length);
  } else {
    // 分两次拷贝
    void *result;

    char *max_write = (char *)buffer->virt_buf1 + buffer->size;
    if (current + length > max_write) {
      size_t first_part = max_write - current;
      result = memcpy(current, data, first_part);
      result = memcpy((char *)buffer->virt_buf1, data + first_part,
                      length - first_part);
    } else {
      result = memcpy(current, data, length);
    }
    check(result != NULL, "Failed to write data into buffer.");
  }

  buffer->write_ptr += length;
  return length;

error:
  return -1;
}

int RingBuffer_write_posix(RingBuffer *buffer, char *data, int length) {
  check(length > 0, "\n\tCannot write %d bytes.", length);

  int available_space = RingBuffer_available_space_posix(buffer);
  check(length <= available_space,
        "\n\t[Refuse to write]: %d request, %d available.", length,
        available_space);

  int actual_write = continuous_write(buffer, data, length);
  check(actual_write != 1, "Failed to write data into buffer.");
  adjust_pointers_posix(buffer);

  return actual_write;

error:
  return 0;
}

static int continuous_read(RingBuffer *buffer, char *target, int amount) {
  char *current = buffer->read_ptr;

  if (buffer->is_continuous) {
    memcpy(target, current, amount);
  } else {
    // 分两次拷贝
    void *result;

    char *max_read = (char *)buffer->virt_buf1 + buffer->size;
    if (current + amount > max_read) {
      size_t first_part = max_read - current;
      result = memcpy(target, current, first_part);
      result = memcpy(target + first_part, (char *)buffer->virt_buf1,
                      amount - first_part);
    } else {
      result = memcpy(target, current, amount);
    }
    check(result != NULL, "Failed to read data from buffer.");
  }

  buffer->read_ptr += amount;
  return amount;

error:
  return -1;
}

int RingBuffer_read_posix(RingBuffer *buffer, char *target, int amount) {
  check(amount > 0, "\n\tCannot read %d bytes.", amount);

  int available_data = RingBuffer_available_data_posix(buffer);
  check(amount <= available_data,
        "\n\t[Refuse to read]: %d available, %d request.", available_data,
        amount);

  int actual_read = continuous_read(buffer, target, amount);
  check(actual_read != -1, "Failed to read data from buffer.");
  adjust_pointers_posix(buffer);

  return actual_read;

error:
  return 0;
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
    log_info("== Using shm_unlink for /dev/shm/%s .", buffer->shm_name);
    shm_unlink(buffer->shm_name);
    free(buffer->shm_name);
  }

  free(buffer);
  log_info("Destroy ringbuffer over.");
}

// 获取内部信息（用于调试）
void RingBuffer_debug_posix(RingBuffer *buffer) {
  log_info("========");
  if (!buffer) {
    debug("Buffer is NULL, now return.");
    return;
  }

  log_info("RingBuffer Debug Info:");
  log_info("  Size: %zu bytes", buffer->size);

  log_info("  Virt_buf1: %p", buffer->virt_buf1);
  log_info("  Virt_buf2: %p (offset: %+ld)", buffer->virt_buf2,
           (char *)buffer->virt_buf2 - (char *)buffer->virt_buf1);
  log_info("  Write ptr: %p (offset: %+ld)", buffer->write_ptr,
           buffer->write_ptr - (char *)buffer->virt_buf1);
  log_info("  Read  ptr: %p (offset: %+ld)", buffer->read_ptr,
           buffer->read_ptr - (char *)buffer->virt_buf1);

  log_info("  Available data : %d bytes",
           RingBuffer_available_data_posix(buffer));
  log_info("  Available space: %d bytes",
           RingBuffer_available_space_posix(buffer));

  // 简单的双映射验证
  // if ((char *)buffer->virt_buf2 > (char *)buffer->virt_buf1) {
  //   log_info("  Verification: Second mapping is after first");
  // }

  // 验证双映射是否工作
  // char *test_addr = (char *)buffer->virt_buf1 + buffer->size;
  // log_info("  Verification:");
  // log_info("    Addr at end   of region1: %p", test_addr);
  // log_info("    Addr at start of region2: %p", buffer->virt_buf2);
  // log_info("    They should point to same physical page");
}
