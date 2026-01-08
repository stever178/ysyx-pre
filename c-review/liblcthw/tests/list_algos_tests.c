#include "minunit.h"
#include <lcthw/list_algos.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_VALUES 15
const int epochs = 1e3;

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
const int num = 5;

List *create_words() {
  srand((unsigned int)time(NULL));

  List *words = List_create();

  for (int i = 0; i < NUM_VALUES; i++) {
    int idx = rand() % num;
    List_push(words, values[idx]);
  }

  return words;
}

int is_sorted(List *words) {
  LIST_FOREACH(words, first, next, cur) {
    if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
      debug("%s %s", (char *)cur->value, (char *)cur->next->value);
      return 0;
    }
  }

  return 1;
}

char *test_bubble_sort() {
  List *words = create_words();
  mu_assert(words->count > 1, "List should have some words.");

  // should work on a list that needs sorting
  List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

  // should work on an already sorted list
  List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

  List_destroy(words);

  // should work on an empty list
  words = List_create();
  List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(is_sorted(words), "Words should be sorted if empty.");
  List_destroy(words);

  return NULL;
}

char *test_merge_sort() {
  List *words = create_words();
  mu_assert(words->count > 1, "List should have some words.");

  // should work on a list that needs sorting
  List *res = List_merge_sort(words, (List_compare)strcmp);
  mu_assert(res != words, "Merge sort should return a new list.(1)");
  mu_assert(is_sorted(res), "Words are not sorted after merge sort.(1)");
  List_destroy(words);

  // should work on an already sorted list
  List *res2 = List_merge_sort(res, (List_compare)strcmp);
  mu_assert(res2 != res, "Merge sort should return a new list.(2)");
  List_destroy(res);
  debug("destroyed res");
  mu_assert(is_sorted(res2), "Words are not sorted after merge sort.(2)");
  List_destroy(res2);

  // should work on an empty list
  words = List_create();
  mu_assert(words->count == 0, "Wrong count on empty list.");
  List *res3 = List_merge_sort(words, (List_compare)strcmp);
  mu_assert(res3->count == 0, "Wrong count on empty list.");
  mu_assert(is_sorted(res3), "Words should be sorted if empty.");
  mu_assert(res3 == words, "Should return same list if empty.");
  if (res3 != words) {
    List_destroy(res3);
  }

  List_destroy(words);
  return NULL;
}

char *test_merge_sort_bottom_up() {
  List *words = create_words();
  mu_assert(words->count > 1, "List should have some words.");

  // should work on a list that needs sorting
  List *res = List_merge_sort_bottom_up(words, (List_compare)strcmp);
  mu_assert(res != words, "Merge sort should return new list.(1)");
  mu_assert(is_sorted(res),
            "Words are not sorted after merge sort from bottom up.(1)");
  List_destroy(words);

  // should work on an already sorted list
  List *res2 = List_merge_sort_bottom_up(res, (List_compare)strcmp);
  mu_assert(res2 != res, "Merge sort should return a new list.(2)");
  List_destroy(res);
  debug("destroyed res");
  mu_assert(is_sorted(res2),
            "Words are not sorted after merge sort from bottom up.(2)");
  List_destroy(res2);

  return NULL;
}

char *test_insert_sort() {
  List *words = create_words();
  mu_assert(words->count > 1, "List should have some words.");

  // should work on an unsorted list
  List *res = List_insert_sort(words, (List_compare)strcmp);
  mu_assert(res != words, "Insert sort should return a new list.(1)");
  mu_assert(is_sorted(res), "Words should be sorted after insert sort.(1)");
  List_destroy(words);

  // should work on a sorted list
  List *res2 = List_insert_sort(res, (List_compare)strcmp);
  mu_assert(res2 != res, "Insert sort should return a new list.(2)");
  List_destroy(res);
  debug("destroyed res");
  mu_assert(is_sorted(res2), "Should still be sorted after insert sort.(2)");
  List_destroy(res2);

  return NULL;
}

char *run_single_sort_test(List_sort sort_func, double *time_record) {
  clock_t start, end;

  for (int i = 0; i < epochs; i++) {
    List *words = create_words();
    start = clock();
    List *result = sort_func(words, (List_compare)strcmp);
    end = clock();
    mu_assert(is_sorted(result), "Words are not sorted after sort");
    if (result != words)
      List_destroy(result);

    List_destroy(words);

    (*time_record) += ((double)(end - start)) / CLOCKS_PER_SEC;
  }

  return NULL;
}

char *test_time_compare() {

  double time_bubble = 0.0;
  double time_merge = 0.0;
  double time_merge_bottom_up = 0.0;
  double time_insert = 0.0;

  run_single_sort_test(List_bubble_sort, &time_bubble);
  run_single_sort_test(List_merge_sort, &time_merge);
  run_single_sort_test(List_merge_sort_bottom_up, &time_merge_bottom_up);
  run_single_sort_test(List_insert_sort, &time_insert);

  debug("\n  After %3d epochs sorting,\n"
        "\t'bubble_sort' used %.3f seconds,\n"
        "\t'merge_sort' used %.3f seconds,\n"
        "\t'merge_sort_bottom_up' used %.3f seconds.\n"
        "\t'insert_sort' used %.3f seconds.\n",
        epochs, time_bubble, time_merge, time_merge_bottom_up, time_insert);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_bubble_sort);
  mu_run_test(test_merge_sort);
  mu_run_test(test_merge_sort_bottom_up);
  mu_run_test(test_insert_sort);

  mu_run_test(test_time_compare);

  debug("\n[END of test]\n");
  return NULL;
}

RUN_TESTS(all_tests);
