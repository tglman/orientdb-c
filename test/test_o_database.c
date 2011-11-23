#include "test_o_database.h"
#include <ctesf.h>
#include "../src/o_database.h"
#include "../src/o_record_raw.h"
#include "test_commons.h"
#include <stdio.h>
#include <string.h>

void test_o_database_new_open_close()
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_open(db, "admin", "admin");
	o_database_close(db);
	o_database_free(db);
}

void test_database_crud_opertation(struct o_database * db, void(*middle)(struct o_database *))
{
	struct o_record * record = o_database_record_new(db);
	int size = strlen("content content");
	o_record_raw_reset(record, "content content", size);
	struct o_record_id *id;
	o_database_save(db, record, &id);

	if (middle != 0)
		middle(db);
	id = o_record_get_id(record);
	struct o_record * load_rec = o_database_load(db, id);

	int load_size;
	char * content = o_record_raw_content(load_rec, &load_size);
	assert_true(size == load_size, "readed have not same size of writed");
	assert_true(memcmp(content, "content content", load_size) == 0, "readed have not same content of writed");
	int test_size = strlen("test test");
	o_record_raw_reset(load_rec, "test test", test_size);
	o_database_save(db, load_rec, 0);
	struct o_record * load_rec2 = o_database_load(db, id);

	content = o_record_raw_content(load_rec2, &load_size);
	assert_true(test_size == load_size, "readed have not same size of writed");
	assert_true(memcmp(content, "test test", load_size) == 0, "readed have not same content of writed");

	o_database_delete(db, load_rec2);

	o_record_release(record);
	o_record_release(load_rec);
	o_record_release(load_rec2);

}

void test_o_database_new_open_crud_close()
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/temp", errorHandler);
	o_database_open(db, "admin", "admin");
	test_database_crud_opertation(db, 0);
	o_database_close(db);
	o_database_free(db);
}

void commit_and_begin(struct o_database * db)
{
	o_database_commit(db);
	o_database_begin_transaction(db);
}
void test_o_database_transaction_commit()
{
	struct o_database_error_handler *errorHandler = o_database_error_handler_new(o_db_error_handler_function, 0);
	struct o_database * db = o_database_new_error_handler("remote:127.0.0.1/temo", errorHandler);
	o_database_open(db, "admin", "admin");
	o_database_begin_transaction(db);
	test_database_crud_opertation(db, commit_and_begin);
	o_database_commit(db);
	o_database_close(db);
	o_database_free(db);
}

void o_database_suite()
{
	ADD_TEST(test_o_database_new_open_close, "Test a database new open close and free");
	ADD_TEST(test_o_database_new_open_crud_close, "Test a database new open write read close and free");
	ADD_TEST(test_o_database_transaction_commit, "Test a database crud operation in a commited transaction");
}
