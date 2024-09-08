#include "minunit.h"
#include <assert.h>
#include <string.h>
#include <lcthw/ringbuffer.h>

static RingBuffer *ringbuffer = NULL;
const int MAX_LEN = 100;

const int length1 = 10;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";


char *test_create()
{
	ringbuffer = RingBuffer_create(MAX_LEN);
    mu_assert(ringbuffer != NULL, "Failed to create ringbuffer.");
    return NULL;
}


char *test_destroy()
{
    RingBuffer_destroy(ringbuffer);
    return NULL;
}


char *test_read_write()
{
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
