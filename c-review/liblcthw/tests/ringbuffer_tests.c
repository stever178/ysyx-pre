#include "minunit.h"
// #include <lcthw/ringbuffer.h>
#include <lcthw/ringbuffer_posix.h>

static RingBuffer *ringbuffer = NULL;
const int MAX_LEN = 100;

const int length1 = 10;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

#define MAX_SIZE 9000

char data1[MAX_SIZE];
char data2[MAX_SIZE];
char target[MAX_SIZE];
int len1 = 3000;
int len2 = 2000;
char data1_value = 'A';
char data2_value = 'B';

char *test_create() {
  ringbuffer = RingBuffer_create_posix(MAX_LEN);
  mu_assert(ringbuffer != NULL, "Failed to create ringbuffer.");
  return NULL;
}

char *test_destroy() {
  RingBuffer_destroy_posix(ringbuffer);
  return NULL;
}

char *test_read_write() {
  int rc = -1;
  char *result = calloc(100, sizeof(char));

  rc = RingBuffer_write_posix(ringbuffer, test1, length1);
  mu_assert(rc == length1, "Wrong on write to buffer");

  rc = RingBuffer_read_posix(ringbuffer, result, length1);
  mu_assert(rc == length1, "Wrong on read to buffer");
  mu_assert(strcmp(test1, result) == 0, "Wrong when memcpy at read");

  free(result);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_read_write);
  // more tests to be added ...
  mu_run_test(test_destroy);

  debug("\n[END of test]\n ");
  return NULL;
}

RUN_TESTS(all_tests);
