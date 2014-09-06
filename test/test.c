#include "suites.h"
#include <stdlib.h>
/*
TEST_INITIALIZE
{
	ADD_SUITE(o_exceptions_suite, "exceptions management tests");
}
*/

int main()
{
	int number_failed;
	Suite *s = suite_create ("orientdb");
	suite_add_tcase (s, o_map_tests());
	suite_add_tcase (s, o_connection_remote_tests());
	suite_add_tcase (s, o_url_resolve_tests());
	suite_add_tcase (s, o_string_buffer_tests());
	suite_add_tcase (s, o_document_value_tests());
	suite_add_tcase (s, o_record_tests());
	suite_add_tcase (s, o_string_printer_tests());
	suite_add_tcase (s, o_document_tests());
	suite_add_tcase (s, o_input_stream_tests());
	suite_add_tcase (s, o_native_lock_tests());
	suite_add_tcase (s, o_database_tests());
	suite_add_tcase (s, o_database_document_tests());
	suite_add_tcase (s, o_database_document_metadata_tests());
	suite_add_tcase (s, o_list_tests());
	suite_add_tcase (s, o_list_record_tests());
	suite_add_tcase (s, o_database_query_tests());
	suite_add_tcase (s, o_database_document_query_tests());
	suite_add_tcase (s, o_pool_tests());
	suite_add_tcase (s, o_output_stream_tests());
	
	SRunner *sr = srunner_create (s);
	srunner_run_all (sr, CK_NORMAL);
	number_failed = srunner_ntests_failed (sr);
	srunner_free (sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

