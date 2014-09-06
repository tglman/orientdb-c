#include "suites.h"
#include "../src/o_record_raw.h"
#include "../src/o_list_record.h"

START_TEST( test_o_list_record_create)
{
	struct o_list_record * rec = o_list_record_new();
	o_list_record_release(rec);
}
END_TEST

START_TEST( test_o_list_record_add_remove)
{
	struct o_list_record * reclis = o_list_record_new();
	struct o_record * rec0 = o_record_raw_new();
	o_list_record_add(reclis, rec0);
	struct o_record * rec1 = o_record_raw_new();
	o_list_record_add(reclis, rec1);
	assert_true(o_list_record_size(reclis) == 2, "the size not is the expected");
	struct o_record * rec3 = o_list_record_get(reclis, 0);
	assert_true(rec3 == rec0, "the first added not is the same of added");
	o_record_release(rec3);
	struct o_record * rec4 = o_list_record_get(reclis, 1);
	assert_true(rec4 == rec1, "the first added not is the same of added");
	o_record_release(rec4);
	assert_true(o_list_record_remove(reclis, rec1) == rec1, "the record returned from remove not is the same of give to");
	assert_true(o_list_record_size(reclis) == 1, "the size not is the expected after remove");
	o_list_record_release(reclis);
	o_record_release(rec0);
	o_record_release(rec1);
}
END_TEST

TCase * o_list_record_tests()
{
	TCase *tc_core = tcase_create ("o_list_record");
	tcase_add_test (tc_core, test_o_list_record_create);
	tcase_add_test (tc_core, test_o_list_record_add_remove);
	return tc_core;
}
