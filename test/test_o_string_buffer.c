#include "suites.h"
#include "../src/o_string_buffer.h"
#include "../src/o_memory.h"
#include <string.h>
#include <stdio.h>

START_TEST (o_string_buffer_test_append_string)
{
	struct o_string_buffer * buffer = o_string_buffer_new();
	o_string_buffer_append(buffer, "test_app");
	o_string_buffer_append(buffer, " app");
	char * content = o_string_buffer_str(buffer);
	assert_true(strcmp(content, "test_app app") == 0, "The content of buffer not is the same appended");
	o_free(content);
	o_string_buffer_free(buffer);
}
END_TEST

START_TEST( o_string_buffer_test_append_integer)
{
	struct o_string_buffer * buffer = o_string_buffer_new();
	o_string_buffer_append(buffer, "test_int ");
	o_string_buffer_append_long(buffer, 30);
	char * content = o_string_buffer_str(buffer);
	assert_true(strcmp(content, "test_int 30") == 0, "The content of buffer not is the same appended");
	o_free(content);
	o_string_buffer_free(buffer);
}
END_TEST

START_TEST(o_string_buffer_test_append_double)
{
	struct o_string_buffer * buffer = o_string_buffer_new();
	o_string_buffer_append(buffer, "test_double ");
	o_string_buffer_append_double(buffer, 30.25);
	char * content = o_string_buffer_str(buffer);
	assert_true(strcmp(content, "test_double 30.25") == 0, "The content of buffer not is the same appended");
	o_free(content);
	o_string_buffer_free(buffer);
}
END_TEST

START_TEST( o_string_buffer_test_append_char)
{
	struct o_string_buffer * buffer = o_string_buffer_new();
	o_string_buffer_append(buffer, "test_char ");
	o_string_buffer_append_char(buffer, 'A');
	char * content = o_string_buffer_str(buffer);
	assert_true(strcmp(content, "test_char A") == 0, "The content of buffer not is the same appended");
	o_free(content);
	o_string_buffer_free(buffer);
}
END_TEST

TCase *  o_string_buffer_tests()
{
	TCase *tc_core = tcase_create ("o_string_buffer");
	tcase_add_test (tc_core, o_string_buffer_test_append_string);
	tcase_add_test (tc_core, o_string_buffer_test_append_integer);
	tcase_add_test (tc_core, o_string_buffer_test_append_double);
	tcase_add_test (tc_core, o_string_buffer_test_append_char);
	return tc_core;

}
