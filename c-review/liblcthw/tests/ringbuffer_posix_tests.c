#include "minunit.h"
#include <lcthw/utils.h>

#include <lcthw/ringbuffer_posix.h>
RingBuffer *(*RingBuffer_create)(int) = RingBuffer_create_posix;
void (*RingBuffer_destroy)(RingBuffer *) = RingBuffer_destroy_posix;
int (*RingBuffer_write)(RingBuffer *, char *, int) = RingBuffer_write_posix;
int (*RingBuffer_read)(RingBuffer *, char *, int) = RingBuffer_read_posix;

static RingBuffer *ringbuffer = NULL;

const int PAGE_SIZE = 4096;
#define MAX_SIZE 9000

char data1[MAX_SIZE];
char data2[MAX_SIZE];
char target[MAX_SIZE];
int len1 = 3000;
int len2 = 2000;
const char data1_value = 'A';
const char data2_value = 'B';

char *test_write(RingBuffer *buffer, char *data, int length) {
  log_info("\nWriting %4d bytes...", length);
  int written = RingBuffer_write_posix(buffer, data, length);
  log_info("Written: %4d bytes.", written);
  // mu_assert(written == length, "Wrong write number after write.");

  RingBuffer_debug_posix(buffer);

  return NULL;
}

char *test_read(RingBuffer *buffer, char *target, char data_expect,
                int length) {
  log_info("\nReading %4d bytes...", length);
  int read = RingBuffer_read_posix(buffer, target, length);
  log_info("Read: %4d bytes.", read);
  // mu_assert(read == length, "Wrong read number after read.");

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
  ringbuffer = RingBuffer_create_posix(PAGE_SIZE);
  mu_assert(ringbuffer != NULL, "Failed to create ringbuffer.");
  RingBuffer_debug_posix(ringbuffer);
  return NULL;
}

char *test_destroy() {
  RingBuffer_destroy_posix(ringbuffer);
  return NULL;
}

char *test_rw() {
  log_info("\n== Read/Write test...");
  char *result;

  result = test_write(ringbuffer, data1, len1);
  if (result != NULL) {
    return result;
  }

  memset(target, 0, sizeof(target));
  result = test_read(ringbuffer, target, data1_value, len1);
  if (result != NULL) {
    return result;
  }

  result = test_write(ringbuffer, data2, len2);
  if (result != NULL) {
    return result;
  }

  memset(target, 0, sizeof(target));
  result = test_read(ringbuffer, target, data2_value, len2);
  if (result != NULL) {
    return result;
  }

  log_info("\n");
  return result;
}

char *test_rw_overflow() {
  log_info("\n== Overflow test...");
  char *result;

  result = test_write(ringbuffer, data1, len1);
  if (result != NULL) {
    return result;
  }

  result = test_write(ringbuffer, data2, len2);
  if (result != NULL) {
    return result;
  }

  memset(target, 0, sizeof(target));
  result = test_read(ringbuffer, target, data1_value, len1);
  if (result != NULL) {
    return result;
  }

  memset(target, 0, sizeof(target));
  result = test_read(ringbuffer, target, data2_value, len2);
  if (result != NULL) {
    return result;
  }

  log_info("\n");
  return result;
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
  RingBuffer_debug_posix(ringbuffer);

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
  RingBuffer_debug_posix(ringbuffer);

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
      log_info(
          "\n[Buffer size: %10d] wr operations using: %.6lf seconds [posix].",
          buffer_size, time_record);
    }

    RingBuffer_destroy(ringbuffer);
  }

  free_string(str1);
  free_string(str2);

  return NULL;
}

char *all_tests() {
  memset(data1, data1_value, sizeof(data1));
  memset(data2, data2_value, sizeof(data2));

  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_rw);
  mu_run_test(test_rw_overflow);
  mu_run_test(test_destroy);

  mu_run_test(test_random_write_read);

  debug("\n[END of test]\n ");
  return NULL;
}

RUN_TESTS(all_tests);
