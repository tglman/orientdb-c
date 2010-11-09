#include "test_o_string_buffer.h"
#include <TestFramework/test.h>
#include "../src/o_string_buffer.h"
#include <string.h>
#include <stdio.h>

void o_string_buffer_test_append_string()
{
	struct o_string_buffer * buffer = o_string_buffer_new();
	o_string_buffer_append(buffer, "test_app");
	o_string_buffer_append(buffer, " app");
	char * content = o_string_buffer_str(buffer);
	assert_true(strcmp(content, "test_app app") == 0, "The content of buffer not is the same appended");
	o_string_buffer_free(buffer);
}

void o_string_buffer_test_append_integer()
{
	struct o_string_buffer * buffer = o_string_buffer_new();
	o_string_buffer_append(buffer, "test_int ");
	o_string_buffer_append_long(buffer, 30);
	char * content = o_string_buffer_str(buffer);
	assert_true(strcmp(content, "test_int 30") == 0, "The content of buffer not is the same appended");
	o_string_buffer_free(buffer);

}

void o_string_buffer_test_append_double()
{
	struct o_string_buffer * buffer = o_string_buffer_new();
	o_string_buffer_append(buffer, "test_double ");
	o_string_buffer_append_double(buffer, 30.25);
	char * content = o_string_buffer_str(buffer);
	assert_true(strcmp(content, "test_double 30.25") == 0, "The content of buffer not is the same appended");
	o_string_buffer_free(buffer);

}

void o_string_buffer_suite()
{
	ADD_TEST(o_string_buffer_test_append_string, "Test an append of string to string buffer");
	ADD_TEST(o_string_buffer_test_append_integer, "Test an append of integer to string buffer");
	ADD_TEST(o_string_buffer_test_append_double, "Test an append of double to string buffer");
}
