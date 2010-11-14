#include "test_o_record.h"
#include "../src/o_record.h"
#include "../src/o_record_id.h"
#include <TestFramework/test.h>

void test_o_record_id_new()
{
	struct o_record_id * id = o_record_id_new_empty();
	o_record_id_free(id);
}

void test_o_record_id_managment()
{
	struct o_record_id * id = o_record_id_new(10, 20);
	assert_true(o_record_id_cluster_id(id) == 10, "not the same cluster id of inserted");
	assert_true(o_record_id_record_id(id) == 20, "not the same record id of inserted");
	o_record_id_free(id);
}

void o_record_id_suite()
{
	ADD_TEST(test_o_record_id_new, " test id creation and destroy");
	ADD_TEST(test_o_record_id_managment, " test id creation with cluster id and rid and retrive it");

}

void test_o_record_new()
{
	struct o_record * rec = o_record_new();
	struct o_record_id * id = o_record_get_id(rec);
	assert_true(o_record_id_cluster_id(id) == -1, "not the same cluster id of default");
	assert_true(o_record_id_record_id(id) == -1, "not the same record id of default");
	assert_true(o_record_version(rec) == 0, "the version not is the default");
	o_record_free(rec);
}

void o_record_suite()
{
	ADD_SUITE(o_record_id_suite, "suite on record id");
	ADD_TEST(test_o_record_new, "test creation of new simple record");
}

