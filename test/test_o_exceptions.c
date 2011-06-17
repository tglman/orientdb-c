#include "test_o_exceptions.h"
#include <ctesf.h>
#include "../src/o_exceptions.h"
#include "../src/o_exception.h"
#include <string.h>
#include <stdio.h>

#define EXCEPTION_MESSAGE "exceptions message"

void test_o_exception_throw()
{
	try
	{
		struct o_exception *ex = o_exception_new(EXCEPTION_MESSAGE, 20);
		throw(ex);
		fail("Error this code must not be executed after exception throw");
	}
	catch(struct o_exception , ex)
	{
		assert_true(strcmp(o_exception_message(ex), EXCEPTION_MESSAGE) == 0, "Exception message not match");
		assert_true(o_exception_code(ex) == 20, "Exception error code not match");
		o_exception_free(ex);
	}

}

void internal_exception()
{
	struct o_exception *ex = o_exception_new(EXCEPTION_MESSAGE, 20);
	throw(ex);
}

void internal_safe_exception()
{
	try
	{
		internal_exception();
		fail("Error this code must not be executed after exception throw");
	}
	catch(struct o_exception ,ex)
	{
		assert_true(strcmp(o_exception_message(ex), EXCEPTION_MESSAGE) == 0, "Exception message not match");
		assert_true(o_exception_code(ex) == 20, "Exception error code not match");
		o_exception_free(ex);
	}
	end_try;
}

void test_o_exceptions_nested()
{
	try
	{
		int check = 0;
		try
		{
			internal_safe_exception();
			check = 1;
			internal_exception();
			fail("Error this code must not be executed after exception throw");
		}
		catch(struct o_exception ,ex)
		{
			if (check == 0)
				fail("Exception scaled up before");
			assert_true(strcmp(o_exception_message(ex), EXCEPTION_MESSAGE) == 0, "Exception message not match");
			assert_true(o_exception_code(ex) == 20, "Exception error code not match");
			o_exception_free(ex);
		}
		end_try;
	}
	catch(struct o_exception ,ex)
	{
		o_exception_free(ex);
		fail("Error on catch invoke");
	}
	end_try;
}

struct o_test_ex
{
	char test;
};

void test_o_exception_catch_up()
{
	try
	{
		try
		{
			struct o_exception *ex = o_exception_new(EXCEPTION_MESSAGE, 20);
			throw(ex);
			fail("Error this code must not be executed after exception throw");
		}
		catch(struct o_test_ex , te)
		{
			fail("Errate catch invoke");
		}
		end_try;
		fail("Errate must jump up");
	}
	catch(struct o_exception ,ex)
	{
		assert_true(strcmp(o_exception_message(ex), EXCEPTION_MESSAGE) == 0, "Exception message not match");
		assert_true(o_exception_code(ex) == 20, "Exception error code not match");
		o_exception_free(ex);
	}
	end_try;
}

void o_exceptions_suite()
{
	ADD_TEST(test_o_exception_throw, "Check if throw catch of exception run correctly");
	ADD_TEST(test_o_exceptions_nested, "Check nested catch an throws");
	ADD_TEST(test_o_exception_catch_up, "Check nested catch throws jump to right up catch");

}
