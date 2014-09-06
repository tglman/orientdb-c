#include "suites.h"
#include "../src/o_record.h"
#include "../src/o_record_id.h"
#include "../src/o_record_factory.h"


void test_o_record_id_new()
{
	struct o_record_id * id = o_record_id_new_empty();
	o_record_id_release(id);
}

void test_o_record_id_managment()
{
	struct o_record_id * id = o_record_id_new(10, 20);
	assert_true(o_record_id_cluster_id(id) == 10, "not the same cluster id of inserted");
	assert_true(o_record_id_record_id(id) == 20, "not the same record id of inserted");
	o_record_id_release(id);
}

void test_o_record_new()
{
	struct o_record * rec = o_record_factory('d');
	struct o_record_id * id = o_record_get_id(rec);
	assert_true(o_record_id_cluster_id(id) == -1, "not the same cluster id of default");
	assert_true(o_record_id_record_id(id) == -1, "not the same record id of default");
	assert_true(o_record_version(rec) == 0, "the version not is the default");
	o_record_release(rec);
}

TCase * o_record_tests()
{
	TCase *tc_core = tcase_create ("o_record");
	tcase_add_test (tc_core, test_o_record_new);
	tcase_add_test (tc_core, test_o_record_id_new);
	tcase_add_test (tc_core, test_o_record_id_managment);
	return tc_core;
}

