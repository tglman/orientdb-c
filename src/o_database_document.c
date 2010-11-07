#include "o_database_document.h"
#include "o_memory.h"
#include "o_database_internal.h"

struct o_database_document
{
	struct o_database database;
};

void o_document_record_writer(struct o_record *record, struct o_string_buffer * buffer)
{

}

struct o_database * o_database_document_to_database(struct o_database_document * db)
{
	return &db->database;
}

struct o_database_document * o_database_document_new(char * connection_url)
{
	return o_database_document_new_error_handler(connection_url, 0);
}

struct o_database_document * o_database_document_new_error_handler(char * connection_url, struct o_database_error_handler * error_handler)
{
	struct o_database_document * new_db = o_malloc(sizeof(struct o_database_document));
	o_database_new_internal(o_database_document_to_database(new_db), connection_url, error_handler);
	new_db->database.o_record_writer = o_document_record_writer;
	return new_db;
}

void o_database_document_open(struct o_database_document * db, char * username, char * password)
{
	o_database_open(o_database_document_to_database(db), username, password);
}

void o_database_document_reset_error_handler(struct o_database_document * db, struct o_database_error_handler *error_handler)
{
	o_database_reset_error_handler(o_database_document_to_database(db), error_handler);
}

void o_database_document_save(struct o_database_document *db, struct o_document * doc)
{
	//o_storage_update_record(db->storage,o_do)
}

void o_database_document_free(struct o_database_document * db)
{
	o_database_free_internal(o_database_document_to_database(db));
	o_free(db);
}
