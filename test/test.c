#include <TestFramework/test.h>
#include "test_map.h"
#include "test_connection_remote.h"
#include "test_exceptions.h"
#include "test_o_url_resolver.h"

void initialize()
{
	ADD_SUITE(o_exceptions_suite, "exceptions management tests");
	ADD_SUITE(o_map_suite, "o_map tests");
	ADD_SUITE(o_connection_remote_suite, "remote connection tests");
	ADD_SUITE(o_url_resolve_suite, "url parsing test");
}
