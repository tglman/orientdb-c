#include "test_o_database.h"
#include <TestFramework/test.h>
#include "../src/o_database.h"
#include <stdio.h>

void o_db_error_handler_function(int code, char * error, void * custom_info)
{
	char message[512];
	sprintf(message, "code:%i message:%s", code, error);
	fail(message);
}

void test_o_database_new_open_close()
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/demo", errorHandler);

	o_database_open(db, "admin", "admin");
	printf("biiiiii");
	fflush(stdout);
	o_database_close(db);
	o_database_free(db);
}

void o_database_suite()
{
	ADD_TEST(test_o_database_new_open_close, "Test a database new open close and free");
}
