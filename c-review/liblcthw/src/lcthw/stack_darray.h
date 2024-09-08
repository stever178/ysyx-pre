#pragma once

#ifndef lcthw_Stack_darray_h
#define lcthw_Stack_darray_h

#include <lcthw/darray.h>

#define Stack_count(A) DArray_count(A)
#define STACK_FOREACH(stack, cur) StackNode *cur; \
	for(i = 0; cur = &stack->contents[i], i < stack->end; i ++) \

#define Stack_last(A) ((A)->contents[(A)->end - 1].value)
#define Stack_first(A) ((A)->contents[0])
#define Stack_end(A) ((A)->end)
#define Stack_count(A) Stack_end(A)
#define Stack_max(A) ((A)->max)

//typedef DArray Stack;
typedef struct StackNode {
	void *value;
} StackNode;
typedef struct Stack {
	int end;
	int max;
	size_t element_size;
	size_t expand_rate;
	StackNode *contents;
} Stack;

void *Stack_create() {
	Stack *array = malloc(sizeof(Stack));
	check_mem(array);

	int initial_max = 1;
	array->max = initial_max;
	check(array->max > 0, "You must set an initial_max > 0.");

	array->contents = calloc(initial_max, sizeof(StackNode));
	check_mem(array->contents);

	array->end = 0;
	array->element_size = 1;
	array->expand_rate = DEFAULT_EXPAND_RATE;
	return array;

error:
	if(array) free(array);
	return NULL;
}

void Stack_destroy(Stack *array) {
	if(array) {
		if(array->contents) free(array->contents);
		free(array);
	}
}

static inline int Stack_resize(Stack *array, size_t newsize)
{
	array->max = newsize;
	check(array->max > 0, "The newsize must be > 0.");

	void *contents = realloc(array->contents, array->max * sizeof(StackNode));
	check_mem(contents);
	array->contents = contents;
	return 0;
error:
	return -1;
}

int Stack_expand(Stack *array)
{
	size_t old_max = array->max;
	check(Stack_resize(array, array->max + array->expand_rate) == 0,
			"Failed to expand array to new size: %d",
			array->max + (int)array->expand_rate);

	memset(array->contents + old_max, 0, array->expand_rate + 1);
	return 0;

error:
	return -1;
}

void Stack_push(Stack *array, void *value) {
	array->contents[array->end].value = value;
	array->end++;

	if(Stack_end(array) >= Stack_max(array)) {
		return Stack_expand(array);
	} else {
		return 0;
	}
}

void *Stack_pop(Stack *array) {
	check(array->end - 1 >= 0, "Attempt to pop from empty array.");

	void *el = Stack_last(array);
    //array->contents[i] = NULL;

	array->end--;
	return el;
error:
	return NULL;
}

void *Stack_peek(Stack *array) {
	return Stack_last(array);
}

#endif
