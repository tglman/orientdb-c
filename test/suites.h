#ifndef SUITES_H_
#define SUITES_H_
#include <ctesf.h>

void o_db_error_handler_function(int code, char * error, void * custom_info);

void o_connection_remote_suite();

/*! \brief Tests suite on document metadata management.
 */
void o_database_document_metadata_suite();

/*! \brief Test on query on document database.
 *
 */
void o_database_document_query_suite();

/*! \brief Document database test suite.
 *
 */
void o_database_document_suite();

void o_database_query_suite();

/*! \brief the suite of test on database.
 *
 */
void o_database_suite();

void o_document_value_suite();

void o_document_suite();

void o_exceptions_suite();

void o_input_stream_suite();

/*! \brief Create a Record list suite.
 *
 */
void o_list_record_suite();

/*! \brief the suite of test on list.
 *
 */
void o_list_suite();

/*! \brief Suite of map test.
 *
 */
void o_map_suite();

/*! \brief basic native lock test suite.
 *
 */
void o_native_lock_suite();


void o_native_socket_selector_suite();

/*! \brief the suite of test on pool
 *
 */
void o_pool_suite();

void o_record_suite();

void o_string_buffer_suite();

/*! \brief The suite  of test on string printer.
 *
 */
void o_string_printer_suite();


void o_url_resolve_suite();

void o_output_stream_suite();

#endif /* SUITES_H_ */
