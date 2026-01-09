#pragma once

#ifndef lcthw_Stack_darray_h
#define lcthw_Stack_darray_h

#include <lcthw/darray.h>

#define Stack_count(A) DArray_count(A)
#define STACK_FOREACH(stack, cur)                                              \
  StackNode *cur;                                                              \
  for (i = 0; cur = stack->contents[i], i < Stack_count(stack); i++)

typedef DArray Stack;

typedef struct StackNode {
  void *value;
} StackNode;

void *Stack_create() {
  Stack *stack = DArray_create(sizeof(StackNode *), 1);
  return stack;
}

void Stack_destroy(Stack *stack) { DArray_clear_destroy(stack); }

void Stack_push(Stack *stack, void *value) {
  StackNode *node = calloc(sizeof(StackNode), 1);
  node->value = value;
  DArray_push(stack, node);
}

void *Stack_pop(Stack *stack) {
  StackNode *node = DArray_pop(stack);
  void *value = node->value;
  free(node);
  return value;
}

void *Stack_peek(Stack *stack) {
  StackNode *node = DArray_last(stack);
  return node->value;
}

#endif
