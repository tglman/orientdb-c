#include "test_o_database_query.h"
#include "../src/o_database_document.h"
#include "../src/o_database.h"
#include "../src/o_document.h"
#include <ctesf.h>
#include "test_commons.h"

void test_o_database_query()
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/demo", errorHandler);
	o_database_open(db, "admin", "admin");
	struct o_query * q = o_query_sql("select * from OUser where name = \'admin\' ");
	struct o_list_record * lr = o_database_query(db, q);

	assert_true(o_list_record_size(lr) == 1, "The query not return the expected number of record");

	o_database_close(db);
	o_database_free(db);

}

void o_database_query_suite()
{
	ADD_TEST(test_o_database_query, "test a query execution.");
}
