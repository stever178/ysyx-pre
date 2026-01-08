// #undef NDEBUG

#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>

#define _MIN(a, b) ((a) < (b) ? (a) : (b))

inline void ListNode_swap(ListNode *a, ListNode *b) {
  void *temp = a->value;
  a->value = b->value;
  b->value = temp;
}

List *List_bubble_sort(List *list, List_compare cmp) {
  int sorted = 1;

  if (List_count(list) <= 1) {
    return list; // already sorted
  }

  do {
    sorted = 1;
    LIST_FOREACH(list, first, next, cur) {
      if (cur->next) {
        if (cmp(cur->value, cur->next->value) > 0) {
          ListNode_swap(cur, cur->next);
          sorted = 0;
        }
      }
    }
  } while (!sorted);

  return list;
}

inline List *Merge_sort_list(List *left, List *right, List_compare cmp) {
  List *result = List_create();
  void *val = NULL;

  while (List_count(left) > 0 || List_count(right) > 0) {
    if (List_count(left) > 0 && List_count(right) > 0) {
      if (cmp(List_first(left), List_first(right)) <= 0) {
        val = List_shift(left);
      } else {
        val = List_shift(right);
      }

      List_push(result, val);
    } else if (List_count(left) > 0) {
      val = List_shift(left);
      List_push(result, val);
    } else if (List_count(right) > 0) {
      val = List_shift(right);
      List_push(result, val);
    }
  }

  assert(List_count(left) == 0);
  assert(List_count(right) == 0);

  return result;
}

inline void print_list(List *list) {
  debug("[");
  LIST_FOREACH(list, first, next, cur) {
    (void)cur;
    debug("%s ", (char *)cur->value);
  }
  debug("]");
}

List *List_merge_sort(List *list, List_compare cmp) {
  if (List_count(list) <= 1) {
    return list;
  }

  debug("List_create left");
  List *left = List_create();
  debug("List_create right");
  List *right = List_create();
  int middle = List_count(list) / 2;

  LIST_FOREACH(list, first, next, cur) {
    if (middle > 0) {
      List_push(left, cur->value);
      debug("[] List_push left: %s", (char *)cur->value);
    } else {
      List_push(right, cur->value);
      debug("[] List_push right: %s", (char *)cur->value);
    }

    middle--;
  }
  assert(List_count(left) + List_count(right) == List_count(list));

  debug("List_create sort_left");
  List *sort_left = List_merge_sort(left, cmp);
  debug("List_create sort_right");
  List *sort_right = List_merge_sort(right, cmp);

  List *merged = Merge_sort_list(sort_left, sort_right, cmp);
  assert(List_count(sort_left) == 0);
  assert(List_count(sort_right) == 0);

  // use the ending condition, think about it
  if (sort_left != left) {
    debug("List_destroy sort_left");
    List_destroy(sort_left);
  }
  if (sort_right != right) {
    debug("List_destroy sort_right");
    List_destroy(sort_right);
  }

  debug("List_destroy left");
  print_list(left);
  List_destroy(left);
  debug("List_destroy right");
  print_list(right);
  List_destroy(right);

  return merged;
}

List *List_merge_sort_bottom_up(List *list, List_compare cmp) {
  if (List_count(list) <= 1) {
    return list;
  }

  int n = List_count(list);
  int size = 1;

  List *result_list = List_cut(list, 0, n - 1);

  while (size < n) {
    for (int left = 0; left < n; left += size * 2) {
      int mid = _MIN(left + size, n - 1);
      int right = _MIN(left + size * 2, n);
      // notice: [left, right)
      if (mid < right) {
        List *left_list = List_cut(result_list, left, mid - 1);
        List *right_list = List_cut(result_list, mid, right - 1);
        List *merged_list = Merge_sort_list(left_list, right_list, cmp);

        List_copy_from_src(result_list, merged_list, left, right - 1);

        List_destroy(left_list);
        List_destroy(right_list);
        List_destroy(merged_list);
      }
    }

    size *= 2;
  }

  return result_list;
}

void Insert_sorted_list(List *list, void *value, List_compare cmp) {
  if (list->count == 0) {
    List_push(list, value);
    return;
  }

  LIST_FOREACH(list, first, next, cur) {
    if (cmp(value, cur->value) < 0) {
      ListNode *node = calloc(1, sizeof(ListNode));
      node->value = value;

      ListNode *cur_prev = cur->prev;

      if (cur_prev) {
        cur_prev->next = node;
      } else {
        list->first = node;
      }
      node->prev = cur_prev;

      node->next = cur;
      cur->prev = node;

      list->count++;
      return;
    }
  }

  List_push(list, value);
}

List *List_insert_sort(List *list, List_compare cmp) {
  if (List_count(list) <= 1) {
    return list;
  }

  List *result_list = List_create();

  LIST_FOREACH(list, first, next, cur) {
    Insert_sorted_list(result_list, cur->value, cmp);
  }

  return result_list;
}
