#ifndef _lcthw_RingBuffer_POSIX_h
#define _lcthw_RingBuffer_POSIX_h

#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

size_t get_page_size();

typedef struct {
  void *virt_buf1; // 第一个映射区域
  void *virt_buf2; // 第二个映射区域（紧接着第一个）
  bool is_continuous;
  int fd; // 用于 mmap 的文件描述符
  bool use_shm;
  char *shm_name;
  size_t size;     // 缓冲区大小（必须是页大小的倍数）
  char *write_ptr; // 写指针（始终在第一个映射区域内）
  char *read_ptr;  // 读指针（始终在第一个映射区域内）
} RingBuffer;

RingBuffer *RingBuffer_create_posix(int length);

void RingBuffer_destroy_posix(RingBuffer *buffer);

int RingBuffer_read_posix(RingBuffer *buffer, char *target, int amount);

int RingBuffer_write_posix(RingBuffer *buffer, char *data, int length);

// int RingBuffer_empty(RingBuffer *buffer);

// int RingBuffer_full(RingBuffer *buffer);

int RingBuffer_available_data_posix(RingBuffer *buffer);

int RingBuffer_available_space_posix(RingBuffer *buffer);

void RingBuffer_debug_posix(RingBuffer *buffer);

void test_posix();

#endif
