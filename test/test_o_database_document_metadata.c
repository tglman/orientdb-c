#include "test_o_database_document_metadata.h"
#include <testframework/test.h>
#include "../src/o_database_document.h"
#include "../src/o_metadata.h"
#include <string.h>
#include <stdio.h>


void o_db_document_error_handler_function_meta(int code, char * error, void * custom_info)
{
	char message[512];
	sprintf(message, "code:%i message:%s", code, error);
	fail(message);
}


void test_o_database_metadata_retrive()
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_document_error_handler_function_meta, 0);
	struct o_database_document * db = o_database_document_new_error_handler("remote:127.0.0.1/demo", errorHandler);
	o_database_document_open(db, "admin", "admin");
	struct o_metadata * mt = o_database_document_metadata(db);
	struct o_schema * schema = o_metadata_get_schema(mt);
	assert_true(schema != 0, "not exist a schema in metadata");
	o_database_document_close(db);
	o_database_document_free(db);
}

void o_database_document_metadata_suite()
{
	ADD_TEST(test_o_database_metadata_retrive, "test a doucment database metadata load.");
}