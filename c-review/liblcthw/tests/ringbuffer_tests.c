#define NPOSIX_

#include "minunit.h"
#include <lcthw/utils.h>

#ifdef NPOSIX_
#include <lcthw/ringbuffer.h>
#else
#include <lcthw/ringbuffer_posix.h>
RingBuffer *(*RingBuffer_create)(int) = RingBuffer_create_posix;
void (*RingBuffer_destroy)(RingBuffer *) = RingBuffer_destroy_posix;
int (*RingBuffer_write)(RingBuffer *, char *, int) = RingBuffer_write_posix;
int (*RingBuffer_read)(RingBuffer *, char *, int) = RingBuffer_read_posix;
#endif

static RingBuffer *ringbuffer = NULL;
const int MAX_LEN = 100;
const int length1 = 10;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

const int PAGE_SIZE = 4096;
#define MAX_SIZE 9000

char data1[MAX_SIZE];
char data2[MAX_SIZE];
char target[MAX_SIZE];
int len1 = 3000;
int len2 = 2000;
const char data1_value = 'A';
const char data2_value = 'B';

char *test_create() {
  ringbuffer = RingBuffer_create(MAX_LEN);
  mu_assert(ringbuffer != NULL, "Failed to create ringbuffer.");
  return NULL;
}

char *test_destroy() {
  RingBuffer_destroy(ringbuffer);
  return NULL;
}

char *test_read_write() {
  int rc = -1;
  char *result = calloc(100, sizeof(char));

  rc = RingBuffer_write(ringbuffer, test1, length1);
  mu_assert(rc == length1, "Wrong on write to buffer");

  rc = RingBuffer_read(ringbuffer, result, length1);
  mu_assert(rc == length1, "Wrong on read to buffer");
  mu_assert(strcmp(test1, result) == 0, "Wrong when memcpy at read");

  free(result);

  return NULL;
}

char *test_w_(char *data, int input_len) {
  int rc = -1;
  log_info("Writing %4d bytes...", input_len);
  rc = RingBuffer_write(ringbuffer, data, input_len);
  log_info("Written: %4d bytes.", rc);

  if (rc == 0) {
    log_info("[Notice] No available space.");
  } else {
    mu_assert(rc == input_len, "Wrong on write to buffer.");
  }

  return NULL;
}

char *test_r_(char *data, int input_len) {
  int rc = -1;
  log_info("Reading %4d bytes...", input_len);
  rc = RingBuffer_read(ringbuffer, target, input_len);
  log_info("Read: %4d bytes.", rc);

  if (rc == 0) {
    log_info("[Notice] No available data.");
  } else {
    mu_assert(rc == input_len, "Wrong on read from buffer.");
    mu_assert(strncmp(data, target, input_len) == 0,
              "Wrong when memcpy at read.");
  }

  return NULL;
}

char *test_random_write_read() {
  log_info("\n== Random write/read test...");

  int upper = 1 << 10;
  char *str1 = gen_random_string(len1 * upper);
  char *str2 = gen_random_string(len1 * upper);

  for (int factor = 1; factor < upper; factor *= 2) {
    int buffer_size = PAGE_SIZE * factor;
    ringbuffer = RingBuffer_create(buffer_size);
    mu_assert(ringbuffer != NULL, "Failed to create ringbuffer.");

    len1 *= 2;
    len2 *= 2;

    char *result;
    clock_t start, end;
    double time_record = .0;

    {
      start = clock();

      result = test_w_(str1, len1);
      if (result != NULL) {
        return result;
      }
      result = test_w_(str1, len1);

      memset(target, 0, sizeof(target));
      result = test_r_(str1, len2);
      if (result != NULL) {
        return result;
      }

      result = test_w_(str2, len1);
      if (result != NULL) {
        return result;
      }

      memset(target, 0, sizeof(target));
      result = test_r_(str1 + len2, len1 - len2);
      if (result != NULL) {
        return result;
      }

      memset(target, 0, sizeof(target));
      result = test_r_(str2, len1);
      if (result != NULL) {
        return result;
      }

      end = clock();
      time_record = ((double)(end - start)) / CLOCKS_PER_SEC;
      log_info("\n[Buffer size: %10d] wr operations using: %.6lf seconds.",
               buffer_size, time_record);
    }

    RingBuffer_destroy(ringbuffer);
  }

  free_string(str1);
  free_string(str2);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_read_write);
  mu_run_test(test_destroy);

  mu_run_test(test_random_write_read);

  debug("\n[END of test]\n ");
  return NULL;
}

RUN_TESTS(all_tests);
