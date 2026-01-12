#ifndef _lcthw_RingBuffer_h
#define _lcthw_RingBuffer_h

#include <lcthw/bstrlib.h>

typedef struct {
  char *buffer;
  int length;
  int start;
  int end;
} RingBuffer;

RingBuffer *RingBuffer_create(int length);

void RingBuffer_destroy(RingBuffer *buffer);

int RingBuffer_read(RingBuffer *buffer, char *target, int amount);

int RingBuffer_write(RingBuffer *buffer, char *data, int length);

int RingBuffer_empty(RingBuffer *buffer);

int RingBuffer_full(RingBuffer *buffer);

int RingBuffer_available_data(RingBuffer *buffer);

int RingBuffer_available_space(RingBuffer *buffer);

bstring RingBuffer_gets(RingBuffer *buffer, int amount);

#define RingBuffer_available_data(B)                                           \
  ((B)->end >= (B)->start ? ((B)->end - (B)->start)                            \
                          : ((B)->length - ((B)->start - 0) + ((B)->end - 0)))

#define RingBuffer_available_space(B)                                          \
  ((B)->length - RingBuffer_available_data(B))

#define RingBuffer_full(B) (RingBuffer_available_data((B)) == (B)->length)

#define RingBuffer_empty(B) (RingBuffer_available_data((B)) == 0)

#define RingBuffer_puts(B, D) RingBuffer_write((B), bdata((D)), blength((D)))

#define RingBuffer_get_all(B)                                                  \
  RingBuffer_gets((B), RingBuffer_available_data((B)))

#define RingBuffer_starts_at(B) ((B)->buffer + (B)->start)

#define RingBuffer_ends_at(B) ((B)->buffer + (B)->end)

#define RingBuffer_commit_read(B, A)                                           \
  ((B)->start = ((B)->start + (A)) % (B)->length)

#define RingBuffer_commit_write(B, A)                                          \
  ((B)->end = ((B)->end + (A)) % (B)->length)

#endif
