#include "test_o_document_value.h"
#include <test.h>
#include "../src/o_document_value.h"
#include "../src/o_document.h"
#include <string.h>

void test_o_document_value_create()
{

	struct o_document_value *val = o_document_value_bool(1);
	assert_true(o_document_value_get_bool(val) == 1, "bool value not is the expected");
	o_document_value_free(val);

	val = o_document_value_int(10);
	assert_true(o_document_value_get_int(val) == 10, "int value not is the expected");
	o_document_value_free(val);

	val = o_document_value_long(10);
	assert_true(o_document_value_get_long(val) == 10, "long value not is the expected");
	o_document_value_free(val);

	val = o_document_value_short(10);
	assert_true(o_document_value_get_short(val) == 10, "short value not is the expected");
	o_document_value_free(val);

	val = o_document_value_string("aaaaaa-");
	assert_true(strcmp(o_document_value_get_string(val), "aaaaaa-") == 0, "string value not is the expected");
	o_document_value_free(val);

	val = o_document_value_float(10.5);
	assert_true(o_document_value_get_float(val) == 10.5, "float value not is the expected");
	o_document_value_free(val);

	val = o_document_value_double(10.5);
	assert_true(o_document_value_get_double(val) == 10.5, "double value not is the expected");
	o_document_value_free(val);

	val = o_document_value_date(10);
	assert_true(o_document_value_get_date(val) == 10, "date value not is the expected");
	o_document_value_free(val);

	//fake pointer only test!!
	struct o_document *doc = o_document_new();
	val = o_document_value_embedded(doc);
	assert_true(o_document_value_get_embedded(val) == doc, "embedded document value not is the expected");
	o_document_value_free(val);
	o_document_release(doc);

	val = o_document_value_link((struct o_record *)doc);
	assert_true(o_document_value_get_link(val) == (struct o_record *)doc, "link document value not is the expected");
	o_document_value_free(val);

	struct o_document_value * arr[3] =
	{ 0, 0, 0 };
	val = o_document_value_array(arr, 3);
	int size;
	struct o_document_value **resarr = o_document_value_get_array(val,&size);
	assert_true(size == 3, "Array size of document value not is the expected");
	int i;
	for (i = 0; i < 3; i++)
		assert_true(arr[i] == resarr[i], "Array content of document value not is the expected");

	o_document_value_free(val);

}

void o_document_value_suite()
{
	ADD_TEST(test_o_document_value_create, "test creation of document value");
}
