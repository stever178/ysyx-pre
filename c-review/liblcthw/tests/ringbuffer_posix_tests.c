#include "minunit.h"
#include <lcthw/ringbuffer_posix.h>

static RingBuffer *ringbuffer = NULL;

const int length = 4096;

#define MAX_SIZE 9000

char data1[MAX_SIZE];
char data2[MAX_SIZE];
char target[MAX_SIZE];
int len1 = 3000;
int len2 = 2000;
char data1_value = 'A';
char data2_value = 'B';

void test_write(RingBuffer *buffer, char *data, int length) {
  printf("\nWriting %4d bytes...\n", length);
  int written = RingBuffer_write_posix(buffer, data, length);
  printf("Written: %4d bytes.\n", written);

  RingBuffer_debug_posix(buffer);
}

char *test_read(RingBuffer *buffer, char *target, char data_expect,
                int length) {
  printf("\nReading %4d bytes...\n", length);
  int read = RingBuffer_read_posix(buffer, target, length);
  printf("Read: %4d bytes.\n", read);
  mu_assert(read == length, "Wrong read number after read.");

  int valid = 1;
  for (int i = 0; i < read; i++) {
    if (target[i] != data_expect) {
      valid = 0;
      break;
    }
  }

  RingBuffer_debug_posix(buffer);

  mu_assert(valid, "Data verification fail in read.");

  return NULL;
}

char *test_create() {
  ringbuffer = RingBuffer_create_posix(length);
  if (!ringbuffer) {
    printf("Failed to create buffer\n");
  }

  RingBuffer_debug_posix(ringbuffer);
  return NULL;
}

char *test_destroy() {
  RingBuffer_destroy_posix(ringbuffer);

  return NULL;
}

char *test_rw() {
  char *result;

  test_write(ringbuffer, data1, len1);
  result = test_read(ringbuffer, target, data1_value, len1);
  memset(target, 0, sizeof(target));

  test_write(ringbuffer, data2, len2);
  result = test_read(ringbuffer, target, data2_value, len2);
  memset(target, 0, sizeof(target));

  printf("\n");
  return result;
}

char *test_rw_overflow() {
  char *result;

  test_write(ringbuffer, data1, len1);
  test_write(ringbuffer, data2, len2);

  result = test_read(ringbuffer, target, data1_value, len1);
  memset(target, 0, sizeof(target));
  result = test_read(ringbuffer, target, data2_value, len2);
  memset(target, 0, sizeof(target));

  printf("\n");
  return result;
}

char *all_tests() {
  memset(data1, data1_value, sizeof(data1));
  memset(data2, data2_value, sizeof(data2));

  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_rw);
  // mu_run_test(test_rw_overflow);

  mu_run_test(test_destroy);

  debug("\n[END of test]\n ");
  return NULL;
}

RUN_TESTS(all_tests);
