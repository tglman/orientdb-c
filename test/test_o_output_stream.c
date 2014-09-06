#include "suites.h"
#include "../src/o_output_stream.h"
#include "../src/o_output_stream_byte.h"

START_TEST( o_output_stream_test_buff_expand)
{
	struct o_output_stream *stream = o_output_stream_byte_buffer();
	o_output_stream_byte_write_bytes(stream,
			"01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789",
			262);
	o_output_stream_free(stream);
}
END_TEST

TCase * o_output_stream_tests()
{
	TCase *tc_core = tcase_create ("o_output_stream");
	tcase_add_test (tc_core, o_output_stream_test_buff_expand);
	return tc_core;
}
