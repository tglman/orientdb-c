#ifndef SUITES_H_
#define SUITES_H_
#include <check.h>

#define assert_true(COND,MSG) ck_assert_msg(COND,MSG)
#define assert_equals(ARG1,ARG2,MSG) ck_assert_msg(ARG1==ARG2,MSG)

void o_db_error_handler_function(int code, char * error, void * custom_info);

TCase * o_connection_remote_tests();

/*! \brief Tests suite on document metadata management.
 */
TCase * o_database_document_metadata_tests();

/*! \brief Test on query on document database.
 *
 */
TCase * o_database_document_query_tests();

/*! \brief Document database test suite.
 *
 */
TCase *  o_database_document_tests();

TCase * o_database_query_tests();

/*! \brief the suite of test on database.
 *
 */
TCase * o_database_tests();

TCase * o_document_value_tests();

TCase * o_document_tests();

void o_exceptions_suite();

TCase * o_input_stream_tests();

/*! \brief Create a Record list suite.
 *
 */
TCase * o_list_record_tests();

/*! \brief the suite of test on list.
 *
 */
TCase * o_list_tests();

/*! \brief Suite of map test.
 *
 */
TCase * o_map_tests();

/*! \brief basic native lock test suite.
 *
 */
TCase* o_native_lock_tests();


void o_native_socket_selector_suite();

/*! \brief the suite of test on pool
 *
 */
TCase* o_pool_tests();

TCase * o_record_tests();

TCase * o_string_buffer_tests();

/*! \brief The suite  of test on string printer.
 *
 */
TCase * o_string_printer_tests();

TCase *  o_url_resolve_tests();

TCase * o_output_stream_tests();

#endif /* SUITES_H_ */
