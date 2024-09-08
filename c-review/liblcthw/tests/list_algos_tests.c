#include "minunit.h"
#include <lcthw/list_algos.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
const int num = 5;
#define NUM_VALUES 100


List *create_words()
{
	srand((unsigned int)time(NULL));

    int i = 0;
    List *words = List_create();

    for(i = 0; i < NUM_VALUES; i++) {
        List_push(words, values[rand() % num]);
    }

    return words;
}

int is_sorted(List *words)
{
    LIST_FOREACH(words, first, next, cur) {
        if(cur->next && strcmp(cur->value, cur->next->value) > 0) {
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
            return 0;
        }
    }

    return 1;
}

char *test_bubble_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    int rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

    // should work on an already sorted list
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

    List_destroy(words);

    // should work on an empty list
    words = List_create(words);
    rc = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(rc == 0, "Bubble sort failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");

    List_destroy(words);
    return NULL;
}

char *test_merge_sort()
{
    List *words = create_words();

    // should work on a list that needs sorting
    List *res = List_merge_sort(words, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    // should work on an already sorted list
    List *res2 = List_merge_sort(res, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort.");

    List_destroy(res2);
    List_destroy(res);

    List_destroy(words);
    return NULL;
}


char *test_time_compare()
{
	int epochs = 1e4;
	clock_t start, end;

	double time_merge = 0.0;
	double time_bubble = 0.0;

    List *words;

	for (int i = 0; i < epochs; i ++) {
    	words = create_words();

		start = clock();
    	List *res = List_merge_sort(words, (List_compare)strcmp);
		end = clock();
		time_merge += ((double) (end - start)) / CLOCKS_PER_SEC;

    	mu_assert(is_sorted(res), "Words are not sorted after merge sort.");
    	List_destroy(words);
	}

	for (int i = 0; i < epochs; i ++) {
    	words = create_words();

		start = clock();
		int rc = List_bubble_sort(words, (List_compare)strcmp);
		end = clock();
		time_bubble += ((double) (end - start)) / CLOCKS_PER_SEC;

    	mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");
    	List_destroy(words);
	}

	debug(" After %3d times sorting, 'bubble' used %.3f seconds, 'merge' used %.3f seconds", epochs, time_bubble, time_merge); 

	return NULL;
}


char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);
	mu_run_test(test_time_compare);

	debug("\n[END of test]\n");
    return NULL;
}

RUN_TESTS(all_tests);
