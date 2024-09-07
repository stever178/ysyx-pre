#pragma once

#ifndef lcthw_Stack_h
#define lcthw_Stack_h

#include <lcthw/list.h>

#define Stack_count(A) ((A)->count)
#define STACK_FOREACH(stack, cur) LIST_FOREACH(stack, first, next, cur)

typedef List Stack;

void *Stack_create() {
	return List_create();
}

void Stack_destroy(Stack *stack) {
	List_destroy(stack);
}

void Stack_push(Stack *stack, void *value) {
	List_push(stack, value);
}

void *Stack_pop(Stack *stack) {
	return List_pop(stack);
}

void *Stack_peek(Stack *stack) {
	return List_last(stack);
}

#endif
