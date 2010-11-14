#include "../src/o_map.h"
#include <TestFramework/test.h>
#include "test_o_map.h"
#include <string.h>
#include <stdio.h>

void test_o_map()
{
	struct o_map *mm = o_map_new();
	void * put_val = (void *) 20;
	o_map_put(mm, "aaa", put_val);
	void * val = o_map_get(mm, "aaa");
	assert_true(val == put_val, "Error on retrieving value from map ");
	o_map_free(mm);
}

void test_o_map_keys_iteration()
{
	struct o_map *mm = o_map_new();
	o_map_put(mm, "aaa", 0);
	o_map_put(mm, "bbb", 0);
	int nk;
	char ** keys = o_map_keys(mm, &nk);
	assert_true(nk == 2, "wrong number of keys");
	assert_true(strcmp(keys[0], "aaa")==0, "wrong first key");
	assert_true(strcmp(keys[1], "bbb")==0, "wrong second key");
	o_map_free(mm);
}

void o_map_suite()
{
	ADD_TEST(test_o_map, "creation and put test");
	ADD_TEST(test_o_map_keys_iteration, "test key add and iterate");
}

