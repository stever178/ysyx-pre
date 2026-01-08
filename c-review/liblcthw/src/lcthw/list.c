#include <lcthw/dbg.h>
#include <lcthw/list.h>

List *List_create() {
  // using calloc, memory is set to zero
  List* result = calloc(1, sizeof(List));
  result->count = 0;
  result->first = NULL;
  result->last = NULL;
  // result->free_flag = false;
  return result;
}

void List_destroy(List *list) {
  // if (list->free_flag) {
  //   return;
  // }

  LIST_FOREACH(list, first, next, cur) {
    if (cur->prev) {
      free(cur->prev);
    }
  }
  if (list->last) {
    free(list->last);
  }

  list->count = 0;
  list->first = NULL;
  list->last = NULL;
  // list->free_flag = true;
  free(list);
}

void List_clear(List *list) {
  LIST_FOREACH(list, first, next, cur) {
    if (cur->value) {
      free(cur->value);
      cur->value = NULL;
    }
  }
}

void List_clear_destroy(List *list) {
  List_clear(list);
  List_destroy(list);
}

void List_push(List *list, void *value) {
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->count++;

error:
  return;
}

void *List_pop(List *list) {
  ListNode *node = list->last;
  return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value) {
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }

  list->count++;

error:
  return;
}

void *List_shift(List *list) {
  ListNode *node = list->first;
  return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node) {
  void *result = NULL;

  check(list->first && list->last, "List is empty.");
  check(node, "node can't be NULL");

  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL,
          "Invalid list, somehow got a first that is NULL.");
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev;
    check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
    list->last->next = NULL;
  } else {
    ListNode *after = node->next;
    ListNode *before = node->prev;
    after->prev = before;
    before->next = after;
  }

  list->count--;
  result = node->value;
  free(node);

error:
  return result;
}

// [start, end]
void *List_cut(List *list, int start, int end) {
  void *result = NULL;
  check(start >= 0, "start %d is too small, should >= 0 \n", start);
  check(end < list->count, "end %d is too large, should < %d \n", end,
        list->count);

  result = List_create();
  int id = 0;
  LIST_FOREACH(list, first, next, cur) {
    if (id >= start) {
      List_push(result, cur->value);
    }
    id++;
    if (id > end)
      return result;
  }

error:
  printf("error return\n");
  return result;
}

// [start, end]
void List_copy_from_src(List *list_dst, List *list_src, int dst_start,
                        int dst_end) {
  check(dst_start >= 0, "dst_start %d is too small, should >= 0 \n", dst_start);
  check(dst_end < list_dst->count, "dst_end %d is too large, should < %d \n",
        dst_end, list_dst->count);

  int id = 0;
  for (ListNode *dst_cur = list_dst->first, *src_cur = list_src->first;
       dst_cur != NULL && src_cur != NULL; dst_cur = dst_cur->next) {
    if (id >= dst_start) {
      dst_cur->value = src_cur->value;
      src_cur = src_cur->next;
    }
    id++;

    if (id > dst_end)
      return;
  }

error:
  return;
}
