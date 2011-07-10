#include "o_database_document.h"
#include "o_database_document_internal.h"
#include "o_memory.h"
#include "o_database_internal.h"
#include "o_metadata_internal.h"
#include <string.h>

struct o_database_document
{
	struct o_database database;
	struct o_metadata * metadata;
	struct o_document_formatter * formatter;
};

struct o_database * o_database_document_to_database(struct o_database_document * db)
{
	return &db->database;
}

struct o_database_document * o_database_document_new(char * connection_url)
{
	return o_database_document_new_error_handler(connection_url, o_database_error_handler_new_default());
}

struct o_database_document * o_database_document_new_error_handler(char * connection_url, struct o_database_error_handler * error_handler)
{
	struct o_database_document * new_db = o_malloc(sizeof(struct o_database_document));
	memset(new_db, 0, sizeof(struct o_database_document));
	o_database_new_internal(o_database_document_to_database(new_db), connection_url, error_handler, DOCUMENT_DB_TYPE);
	return new_db;
}

void o_database_document_open(struct o_database_document * db, char * username, char * password)
{
	o_database_open(o_database_document_to_database(db), username, password);
}

void o_database_document_close(struct o_database_document * db)
{
	o_database_close(o_database_document_to_database(db));
}

void o_database_document_reset_error_handler(struct o_database_document * db, struct o_database_error_handler *error_handler)
{
	o_database_reset_error_handler(o_database_document_to_database(db), error_handler);
}

int o_database_document_save(struct o_database_document *db, struct o_document * doc, struct o_record_id **id)
{
	return o_database_save(o_database_document_to_database(db), o_document_o_record(doc), id);
}

void o_database_document_delete(struct o_database_document * db, struct o_document * doc)
{
	o_database_delete(o_database_document_to_database(db), o_document_o_record(doc));
}

struct o_document * o_database_document_load(struct o_database_document * db, struct o_record_id * id)
{
	struct o_record * record = o_database_load(o_database_document_to_database(db), id);
	if (o_record_type(record) != DOCUMENT_RECORD_TYPE)
	{
		//TODO:notify error.
		return 0;
	}
	return (struct o_document *) record;
}

struct o_metadata * o_database_document_metadata(struct o_database_document * db)
{
	if (db->metadata == 0)
	{
		struct o_record * meta = o_database_metadata(o_database_document_to_database(db));
		db->metadata = o_metadata_from_document((struct o_document *) meta);
	}
	return db->metadata;
}

struct o_document_rh
{
	struct o_database_document * db;
	struct o_list_document * list;
};

void o_query_engine_document_listener(void * add_info, struct o_record *record)
{
	struct o_document_rh * docr = (struct o_document_rh *) add_info;
	o_list_document_add(docr->list, (struct o_document *) record);

}

struct o_list_document * o_database_document_query(struct o_database_document * db, struct o_query * query)
{
	o_database_context_database_init(db);
	struct o_document_rh rh;
	rh.db = db;
	rh.list = o_list_document_new();
	o_database_query_internal(o_database_document_to_database(db), query, 0, o_query_engine_document_listener, &rh);
	o_database_context_database_init(0);
	return rh.list;
}

void o_database_document_free(struct o_database_document * db)
{
	o_database_free_internal(o_database_document_to_database(db));
	if (db->metadata != 0)
		o_metadata_free(db->metadata);
	o_free(db);
}

struct o_document_formatter * o_database_document_get_formatter(struct o_database_document * db)
{
	if (db->formatter == 0)
	{
		db->formatter = o_document_formatter_factory_default();
	}
	return db->formatter;
}
