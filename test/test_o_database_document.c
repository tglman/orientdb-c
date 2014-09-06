#include "suites.h"
#include "../src/o_database_document.h"
#include "../src/o_document_value.h"
#include <string.h>
#include <stdio.h>


START_TEST(test_o_database_document_open_save_load_close)
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database_document * db = o_database_document_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_document_open(db, "admin", "admin");
	struct o_document * doc = o_document_new();
	o_document_field_set(doc, "prova", o_document_value_string("prova"));
	struct o_record_id * id;
	o_database_document_save(db, doc, &id);
	struct o_document *readed_doc = o_database_document_load(db, id);
	struct o_document_value* readed_val = o_document_field_get(doc, "prova");
	assert_true(strcmp(o_document_value_get_string(readed_val), "prova") == 0, "the writed document not is the same of loaded");
	o_document_release(doc);
	o_document_release(readed_doc);
	o_database_document_close(db);
	o_database_document_free(db);
}
END_TEST

TCase * o_database_document_tests()
{
	TCase *tc_core = tcase_create ("o_database_document");
	tcase_add_test (tc_core, test_o_database_document_open_save_load_close);
	return tc_core;
}
