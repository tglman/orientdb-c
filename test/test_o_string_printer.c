#include "suites.h"
#include "../src/o_string_printer.h"
#include "../src/o_string_printer_stream.h"
#include "../src/o_output_stream_byte.h"
#include "../src/o_memory.h"
#include <string.h>
#include <stdio.h>

START_TEST(o_string_buffer_test_print)
{
	struct o_output_stream *o = o_output_stream_byte_buffer();
	struct o_string_printer * buffer = o_string_printer_stream_new(o);
	o_string_printer_print(buffer, "test_app");
	o_string_printer_print(buffer, " app");
	o_string_printer_flush(buffer);
	int size;
	unsigned char * content = o_output_stream_byte_content(o, &size);
	char * mem = o_malloc(size + 1);
	memcpy(mem, content, size);
	mem[size] = 0;
	assert_true(strcmp(mem, "test_app app") == 0, "The content of buffer not is the same appended");
	o_string_printer_free(buffer);
	o_output_stream_free(o);
	o_free(mem);
}
END_TEST

START_TEST( o_string_buffer_test_print_integer)
{
	struct o_output_stream *o = o_output_stream_byte_buffer();
	struct o_string_printer * buffer = o_string_printer_stream_new(o);
	o_string_printer_print(buffer, "test_int ");
	o_string_printer_print_long(buffer, 30);
	o_string_printer_flush(buffer);
	int size;
	unsigned char * content = o_output_stream_byte_content(o, &size);
	char * mem = o_malloc(size + 1);
	memcpy(mem, content, size);
	mem[size] = 0;
	assert_true(strcmp(mem, "test_int 30") == 0, "The content of buffer not is the same appended");
	o_string_printer_free(buffer);
	o_output_stream_free(o);
	o_free(mem);
}
END_TEST

START_TEST(o_string_buffer_test_print_double)
{
	struct o_output_stream *o = o_output_stream_byte_buffer();
	struct o_string_printer * buffer = o_string_printer_stream_new(o);
	o_string_printer_print(buffer, "test_double ");
	o_string_printer_print_double(buffer, 30.25);
	o_string_printer_flush(buffer);
	int size;
	unsigned char * content = o_output_stream_byte_content(o, &size);
	char * mem = o_malloc(size + 1);
	memcpy(mem, content, size);
	mem[size] = 0;
	assert_true(strcmp(mem, "test_double 30.25") == 0, "The content of buffer not is the same appended");
	o_string_printer_free(buffer);
	o_output_stream_free(o);
	o_free(mem);
}
END_TEST

START_TEST( o_string_buffer_test_print_char)
{
	struct o_output_stream *o = o_output_stream_byte_buffer();
	struct o_string_printer * buffer = o_string_printer_stream_new(o);
	o_string_printer_print(buffer, "test_char ");
	o_string_printer_print_char(buffer, 'A');
	o_string_printer_flush(buffer);
	int size;
	unsigned char * content = o_output_stream_byte_content(o, &size);
	char * mem = o_malloc(size + 1);
	memcpy(mem, content, size);
	mem[size] = 0;
	assert_true(strcmp(mem, "test_char A") == 0, "The content of buffer not is the same appended");
	o_string_printer_free(buffer);
	o_output_stream_free(o);
	o_free(mem);
}
END_TEST

TCase * o_string_printer_tests()
{
	TCase *tc_core = tcase_create ("o_string_buffer");
	tcase_add_test (tc_core, o_string_buffer_test_print);
	tcase_add_test (tc_core, o_string_buffer_test_print_integer);
	tcase_add_test (tc_core, o_string_buffer_test_print_double);
	tcase_add_test (tc_core, o_string_buffer_test_print_char);
	return tc_core;
}
