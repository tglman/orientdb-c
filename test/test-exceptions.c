#include "test-exceptions.h"
#include <TestFramework/test.h>
#include "../src/o_exceptions.h"
#include "../src/o_exception.h"
#include <string.h>

#define EXCEPTION_MESSAGE "exceptions message"

void test_o_exception_throw()
{
	try
	{
		struct o_exception *ex = o_exception_new(EXCEPTION_MESSAGE, 20);
		throw(ex);
	}
	catch(struct o_exception , ex)
	{
		assert_true(strcmp(o_exception_message(ex), EXCEPTION_MESSAGE) == 0, "Exception message not match");
		assert_true(o_exception_code(ex) == 20, "Exception error code not match");
		o_exception_free(ex);
	}

}

void test_o_exceptions_nested()
{

}

void o_exceptions_suite()
{
	ADD_TEST(test_o_exception_throw, "Check if throw catch of exception run correctly");
	ADD_TEST(test_o_exceptions_nested, "Check nested catch an throws");
}
