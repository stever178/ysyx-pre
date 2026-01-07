#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>

#define _MIN(a, b) ((a) < (b) ? (a) : (b))

inline void ListNode_swap(ListNode *a, ListNode *b) {
  void *temp = a->value;
  a->value = b->value;
  b->value = temp;
}

int List_bubble_sort(List *list, List_compare cmp) {
  int sorted = 1;

  if (List_count(list) <= 1) {
    return 0; // already sorted
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

  return 0;
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

  return result;
}

List *List_merge_sort(List *list, List_compare cmp) {
  if (List_count(list) <= 1) {
    return list;
  }

  List *left = List_create();
  List *right = List_create();
  int middle = List_count(list) / 2;

  LIST_FOREACH(list, first, next, cur) {
    if (middle > 0) {
      List_push(left, cur->value);
    } else {
      List_push(right, cur->value);
    }

    middle--;
  }

  List *sort_left = List_merge_sort(left, cmp);
  List *sort_right = List_merge_sort(right, cmp);

  // use the ending condition, think about it
  if (sort_left != left)
    List_destroy(left);
  if (sort_right != right)
    List_destroy(right);

  return Merge_sort_list(sort_left, sort_right, cmp);
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
