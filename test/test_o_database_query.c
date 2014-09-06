#include "suites.h"
#include "../src/o_database_document.h"
#include "../src/o_database.h"
#include "../src/o_document.h"
#include <stdio.h>

START_TEST( test_o_database_query)
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_open(db, "admin", "admin");
	struct o_query * q = o_query_sql("select * from OUser where name = \'admin\' ");
	struct o_list_record * lr = o_database_query(db, q);

	assert_true(o_list_record_size(lr) == 1, "The query not return the expected number of record");
	o_list_record_release(lr);
	o_query_free(q);
	o_database_close(db);
	o_database_free(db);
}
END_TEST


START_TEST( test_o_database_multi_query)
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_open(db, "admin", "admin");
	struct o_query * q = o_query_sql("select * from OUser where name = \'admin\' ");
	struct o_list_record * lr = o_database_query(db, q);

	assert_true(o_list_record_size(lr) == 1, "The query not return the expected number of record");
	struct o_query * q1 = o_query_sql("select * from OUser where name = \'admin\' ");
	struct o_list_record * lr1 = o_database_query(db, q);
	assert_true(o_list_record_size(lr1) == 1, "The query not return the expected number of record");
	o_list_record_release(lr);
	o_query_free(q);
	o_list_record_release(lr1);
	o_query_free(q1);
	o_database_close(db);
	o_database_free(db);
}
END_TEST

START_TEST(  test_o_database_query_insert)
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_open(db, "admin", "admin");
	struct o_query * qi = o_query_sql("insert into OUser(name,status,password) values ('testUser','active','test')");
	struct o_list_record * lri = o_database_query(db, qi);
	struct o_query * q = o_query_sql("select * from OUser where name = \'testUser\' ");
	struct o_list_record * lr = o_database_query(db, q);
	assert_true(o_list_record_size(lr) == 1, "The query not return the expected number of record");
	struct o_record *rec = o_list_record_get(lr, 0);
	o_database_delete(db, rec);
	o_record_release(rec);
	o_list_record_release(lri);
	o_list_record_release(lr);
	o_query_free(q);
	o_query_free(qi);
	o_database_close(db);
	o_database_free(db);

}
END_TEST

START_TEST( test_o_database_query_update)
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_open(db, "admin", "admin");
	struct o_query * qi = o_query_sql("insert into OUser(name,status,password) values ('testUser','active','test')");
	struct o_list_record * lri = o_database_query(db, qi);
	struct o_query * qu = o_query_sql("update OUser set name='testUser1' where name = \'testUser\' ");
	struct o_list_record * lru = o_database_query(db, qu);
	struct o_query * q = o_query_sql("select * from OUser where name = \'testUser1\' ");
	struct o_list_record * lr = o_database_query(db, q);
	assert_true(o_list_record_size(lr) == 1, "The query not return the expected number of record");
	struct o_record *rec = o_list_record_get(lr, 0);
	o_database_delete(db, rec);
	o_record_release(rec);
	o_list_record_release(lri);
	o_list_record_release(lru);
	o_list_record_release(lr);
	o_query_free(qi);
	o_query_free(qu);
	o_query_free(q);
	o_database_close(db);
	o_database_free(db);
}
END_TEST

TCase * o_database_query_tests()
{
	TCase *tc_core = tcase_create ("o_database");
	tcase_add_test (tc_core, test_o_database_query);
	tcase_add_test (tc_core, test_o_database_query_insert);
	tcase_add_test (tc_core, test_o_database_query_update);
	tcase_add_test (tc_core, test_o_database_multi_query);
	return tc_core;
}
