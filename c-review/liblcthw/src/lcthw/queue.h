#pragma once

#ifndef lcthw_Queue_h
#define lcthw_Queue_h

#include <lcthw/list.h>

#define Queue_count(A) ((A)->count)
#define QUEUE_FOREACH(queue, cur) LIST_FOREACH(queue, first, next, cur)

typedef List Queue;

void *Queue_create() {
	return List_create();
}

void Queue_destroy(Queue *queue) {
	List_destroy(queue);
}

void Queue_send(Queue *queue, void *value) {
	List_push(queue, value);
}

void *Queue_recv(Queue *queue) {
	return List_shift(queue);
}

void *Queue_peek(Queue *queue) {
	return List_first(queue);
}

#endif
