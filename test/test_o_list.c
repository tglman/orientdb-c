#include "suites.h"
#include "../src/o_list.h"

START_TEST( test_o_list_create_destroy)
{
	struct o_list * list = o_list_new();
	o_list_free(list);
}
END_TEST

START_TEST( test_o_list_functions)
{
	struct o_list * list = o_list_new();
	o_list_add(list, (void *) 10);
	o_list_add(list, (void *) 20);
	assert_true(o_list_size(list) == 2, "the size of list not is the expected");
	assert_true(((long) o_list_get(list, 0)) == 10, "the first value retrieve is not the same of insert");
	assert_true(((long) o_list_get(list, 1)) == 20, "the second value retrieve is not the same of insert");
	o_list_remove(list, (void *) 20);
	assert_true(o_list_size(list) == 1, "after remove the size of list not is the expected");
	assert_true(((long) o_list_get(list, 1)) == 0, "the second value not removed as asked.");
	o_list_free(list);
}
END_TEST

START_TEST( test_o_list_iterator)
{
	struct o_list * list = o_list_new();
	o_list_add(list, (void *) 10);
	o_list_add(list, (void *) 20);
	struct o_list_iterator * iter = o_list_begin(list);
	assert_true(o_list_iterator_current(iter) == (void *) 10, "First value not is the same of insert");
	assert_true(o_list_iterator_next(iter), "Iterator Forward fail");
	assert_true(o_list_iterator_current(iter) == (void *) 20, "Second value not is the same of insert");
	assert_true(!o_list_iterator_next(iter), "Second Iterator Forward fail");
	assert_true(o_list_iterator_prev(iter), "Iterator backward fail");
	assert_true(o_list_iterator_current(iter) == (void *) 10, "First value after back fail");
	o_list_iterator_free(iter);
	struct o_list_iterator * back_iter = o_list_end(list);
	assert_true(o_list_iterator_current(back_iter) == (void *) 20, "Second value not is the same of insert");
	assert_true(o_list_iterator_next(back_iter), "Iterator Forward fail");
	assert_true(o_list_iterator_current(back_iter) == (void *) 10, "First value not is the same of insert");
	assert_true(!o_list_iterator_next(back_iter), "Second Iterator Forward fail");
	assert_true(o_list_iterator_prev(back_iter), "Iterator backward fail");
	assert_true(o_list_iterator_current(back_iter) == (void *) 20, "First value after back fail");
	o_list_iterator_free(back_iter);
	o_list_free(list);
}
END_TEST

TCase * o_list_tests()
{
	TCase *tc_core = tcase_create ("o_list");
	tcase_add_test (tc_core, test_o_list_create_destroy);
	tcase_add_test (tc_core, test_o_list_functions);
	tcase_add_test (tc_core, test_o_list_iterator);
	return tc_core;
}
