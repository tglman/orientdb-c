#include "suites.h"
#include "../src/o_database_document.h"
#include "../src/o_metadata.h"
#include <string.h>
#include <stdio.h>


START_TEST(test_o_database_metadata_retrive)
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database_document * db = o_database_document_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_document_open(db, "admin", "admin");
	struct o_metadata * mt = o_database_document_metadata(db);
	struct o_schema * schema = o_metadata_get_schema(mt);
	assert_true(schema != 0, "not exist a schema in metadata");
	o_database_document_close(db);
	o_database_document_free(db);
}
END_TEST

TCase * o_database_document_metadata_tests()
{
	TCase *tc_core = tcase_create ("o_database_document_metadata");
	tcase_add_test (tc_core, test_o_database_metadata_retrive);
	return tc_core;
}
