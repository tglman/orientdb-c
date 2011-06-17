#include "test_o_list_record.h"
#include <ctesf.h>
#include "../src/o_record_raw.h"
#include "../src/o_list_record.h"

void test_o_list_record_create()
{
	struct o_list_record * rec = o_list_record_new();
	o_list_record_release(rec);
}

void test_o_list_record_add_remove()
{
	struct o_list_record * reclis = o_list_record_new();
	struct o_record * rec0 = o_record_raw_new();
	o_list_record_add(reclis, rec0);
	struct o_record * rec1 = o_record_raw_new();
	o_list_record_add(reclis, rec1);
	assert_true(o_list_record_size(reclis) == 2, "the size not is the expected");
	assert_true(o_list_record_get(reclis, 0) == rec0, "the first added not is the same of added");
	assert_true(o_list_record_get(reclis, 1) == rec1, "the first added not is the same of added");
	assert_true(o_list_record_remove(reclis, rec1) == rec1, "the record returned from remove not is the same of give to");
	assert_true(o_list_record_size(reclis) == 1, "the size not is the expected after remove");
	o_list_record_release(reclis);
}

void o_list_record_suite()
{
	ADD_TEST(test_o_list_record_create, "Base test of list creation and destroy");
	ADD_TEST(test_o_list_record_add_remove, "Test of add remove and size");
}
