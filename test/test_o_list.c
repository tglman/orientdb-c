#include "test_o_list.h"
#include "../src/o_list.h"
#include <test.h>

void test_o_list_create_destroy()
{
	struct o_list * list = o_list_new();
	o_list_free(list);
}

void test_o_list_functions()
{
	struct o_list * list = o_list_new();
	o_list_add(list,(void *)10);
	o_list_add(list,(void *)20);
	assert_true(((long)o_list_get(list,0))==10,"the first value retrieve is not the same of insert");
	assert_true(((long)o_list_get(list,1))==20,"the second value retrieve is not the same of insert");
	o_list_free(list);
}

void o_list_suite()
{
	ADD_TEST(test_o_list_create_destroy,"base list create and destroy test");
	ADD_TEST(test_o_list_create_destroy,"list function test");
}
