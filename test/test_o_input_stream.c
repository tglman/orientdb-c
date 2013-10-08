#include "suites.h"
#include "../src/o_input_stream.h"

void test_o_input_simple_read()
{
	unsigned char bytes[] =
	{ 1, 2, 3, -1, -1 };
	struct o_input_stream * inp = o_input_stream_new_bytes(bytes, 5);
	assert_true(o_input_stream_read(inp) == 1, "Error reading first byte");
	assert_true(o_input_stream_read(inp) == 2, "Error reading second byte");
	assert_true(o_input_stream_read(inp) == 3, "Error reading third byte");
	assert_true(o_input_stream_read(inp) == (int) (unsigned char) -1, "Error reading forty byte");
	assert_true(o_input_stream_read(inp) != -1, "Error reading -1 char content");
	assert_true(o_input_stream_read(inp) == -1, "Error reading end byte");
	o_input_stream_free(inp);
}
void test_o_input_bytes_read()
{
	unsigned char bytes[] =
	{ 1, 2, 3 };
	struct o_input_stream * inp = o_input_stream_new_bytes(bytes, 3);
	unsigned char byte_fill[2];

	assert_true(o_input_stream_read_bytes(inp, byte_fill, 2) == 2, "wrong number of byte read");
	assert_true(byte_fill[0] == 1, "Error reading first byte");
	assert_true(byte_fill[1] == 2, "Error second first byte");
	assert_true(o_input_stream_read_bytes(inp, byte_fill, 2) == 1, "wrong number of byte read");
	assert_true(byte_fill[0] == 3, "Error reading first byte");
	o_input_stream_free(inp);

}

void o_input_stream_suite()
{
	ADD_TEST(test_o_input_simple_read, "Test simple read from an byte array input stream");
	ADD_TEST(test_o_input_bytes_read, "Test multiple bytes read from an byte array input stream");

}
