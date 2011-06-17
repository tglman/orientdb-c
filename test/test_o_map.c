#include "../src/o_map_string.h"
#include <ctesf.h>
#include "test_o_map.h"
#include <string.h>
#include <stdio.h>

void test_o_map()
{
	struct o_map_string *mm = o_map_string_new();
	void * put_val = (void *) 20;
	o_map_string_put(mm, "aaa", put_val);
	void * val = o_map_string_get(mm, "aaa");
	assert_true(val == put_val, "Error on retrieving value from map ");
	o_map_string_free(mm);
}

void test_o_map_keys_iteration()
{
	struct o_map_string *mm = o_map_string_new();
	o_map_string_put(mm, "aaa", 0);
	o_map_string_put(mm, "bbb", 0);
	int nk;
	char ** keys = o_map_string_keys(mm, &nk);
	assert_true(nk == 2, "wrong number of keys");
	assert_true(strcmp(keys[0], "aaa") == 0, "wrong first key");
	assert_true(strcmp(keys[1], "bbb") == 0, "wrong second key");
	o_map_string_free(mm);
}

void test_o_map_add_clear()
{
	struct o_map_string *mm = o_map_string_new();
	o_map_string_put(mm, "cccc", 0);
	o_map_string_put(mm, "sss", 0);
	int size = o_map_string_size(mm);
	assert_true(size == 2, "not correct size after insert");
	o_map_string_clear(mm);
	size = o_map_string_size(mm);
	assert_true(size == 0, "not correct size after clear");

}

void o_map_suite()
{
	ADD_TEST(test_o_map, "creation and put test");
	ADD_TEST(test_o_map_keys_iteration, "test key add and iterate");
	ADD_TEST(test_o_map_add_clear, "test key add and iterate");
}

