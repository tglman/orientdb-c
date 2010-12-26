#include <TestFramework/test.h>
#include "test_o_map.h"
#include "test_o_connection_remote.h"
#include "test_o_exceptions.h"
#include "test_o_url_resolver.h"
#include "test_o_string_buffer.h"
#include "test_o_document_value.h"
#include "test_o_record.h"
#include "test_o_document.h"
#include "test_o_input_stream.h"
#include "test_o_string_printer.h"
#include "test_o_database.h"
#include "test_o_native_lock.h"

void initialize()
{
	ADD_SUITE(o_exceptions_suite, "exceptions management tests");
	ADD_SUITE(o_map_suite, "o_map tests");
	ADD_SUITE(o_connection_remote_suite, "remote connection tests");
	ADD_SUITE(o_url_resolve_suite, "url parsing tests");
	ADD_SUITE(o_string_buffer_suite, "string buffer tests");
	ADD_SUITE(o_document_value_suite, "document value tests");
	ADD_SUITE(o_record_suite, "simple record tests");
	ADD_SUITE(o_string_printer_suite, "printer test");
	ADD_SUITE(o_document_suite, "document tests");
	ADD_SUITE(o_input_stream_suite, "input stream test");
	ADD_SUITE(o_native_lock_suite, "basic native lock test suite");
	ADD_SUITE(o_database_suite, "test on base database");
}
