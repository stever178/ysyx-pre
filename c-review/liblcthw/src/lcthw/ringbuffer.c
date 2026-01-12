#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>

RingBuffer *RingBuffer_create(int length) {
  RingBuffer *buffer = NULL;

  buffer = calloc(1, sizeof(RingBuffer));
  check(buffer != NULL, "Failed to alloc memory for buffer");

  buffer->length = length + 1;
  buffer->start = 0;
  buffer->end = 0;
  buffer->buffer = calloc(buffer->length, 1);
  check(buffer != NULL, "Failed to alloc memory for buffer");

error:
  return buffer;
}

void RingBuffer_destroy(RingBuffer *buffer) {
  if (buffer) {
    free(buffer->buffer);
    free(buffer);
  }
}

int RingBuffer_write(RingBuffer *buffer, char *data, int length) {
  if (RingBuffer_available_data(buffer) == 0) {
    buffer->start = buffer->end = 0;
  }

  check(length > 0, "\n\tNeed more than 0 for write, you gave: %d ", length);
  check(length <= RingBuffer_available_space(buffer),
        "\n\tNot enough space: %d request, %d available", length,
        RingBuffer_available_space(buffer));

  void *result;
  int max_write = buffer->length - buffer->end;
  if (length > max_write) {
    size_t first_part = max_write;
    result = memcpy(RingBuffer_ends_at(buffer), data, first_part);
    result = memcpy(buffer->buffer, data + first_part, length - first_part);
  } else {
    result = memcpy(RingBuffer_ends_at(buffer), data, length);
  }
  check(result != NULL, "Failed to write data into buffer.");

  RingBuffer_commit_write(buffer, length);

  return length;

error:
  return 0;
}

int RingBuffer_read(RingBuffer *buffer, char *target, int amount) {
  check(amount > 0, "\n\tNeed more than 0 for read, you gave: %d ", amount);
  check(amount <= RingBuffer_available_data(buffer),
        "\n\tNot enough in the buffer: has %d, needs %d",
        RingBuffer_available_data(buffer), amount);

  void *result;
  int max_read = buffer->length - buffer->start;
  if (amount > max_read) {
    size_t first_part = max_read;
    result = memcpy(target, RingBuffer_starts_at(buffer), first_part);
    result = memcpy(target + first_part, buffer->buffer, amount - first_part);
  } else {
    result = memcpy(target, RingBuffer_starts_at(buffer), amount);
  }
  check(result != NULL, "Failed to write buffer into data.");

  RingBuffer_commit_read(buffer, amount);

  if (buffer->end == buffer->start) {
    buffer->start = buffer->end = 0;
  }

  return amount;

error:
  return 0;
}

bstring RingBuffer_gets(RingBuffer *buffer, int amount) {
  check(amount > 0, "\n\tNeed more than 0 for gets, you gave: %d ", amount);
  check_debug(amount <= RingBuffer_available_data(buffer),
              "\n\tNot enough in the buffer.");

  bstring result = blk2bstr(RingBuffer_starts_at(buffer), amount);
  check(result != NULL, "Failed to create gets result.");
  check(blength(result) == amount, "Wrong result length.");

  RingBuffer_commit_read(buffer, amount);
  assert(RingBuffer_available_data(buffer) >= 0 && "Error in read commit.");

  return result;
error:
  return NULL;
}
