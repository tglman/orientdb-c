#include "suites.h"
#include "../src/o_database_document.h"

START_TEST( test_o_database_document_query)
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database_document * dd = o_database_document_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_document_open(dd, "admin", "admin");

	o_database_document_metadata(dd);


	struct o_query * q = o_query_sql("select * from OUser where name = 'admin' ");
	struct o_list_document * ld = o_database_document_query(dd, q);

	assert_true(o_list_document_size(ld) == 1, "The query not return the expected number of record");
	o_query_free(q);
	o_list_document_free(ld);
	o_database_document_close(dd);
	o_database_document_free(dd);
}
END_TEST

TCase * o_database_document_query_tests()
{
	TCase *tc_core = tcase_create ("o_database_document_query");
	tcase_add_test (tc_core, test_o_database_document_query);
	return tc_core;
}
