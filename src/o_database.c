#include "o_database.h"
#include "o_database_internal.h"
#include "o_database_error_handler.h"
#include "o_memory.h"
#include <string.h>
#include "o_engine.h"
#include "o_storage.h"
#include "o_url_resolver.h"
#include "o_raw_buffer.h"
#include "o_input_stream.h"
#include "o_record_factory.h"
#include "o_record_raw.h"
#include "o_exceptions.h"
#include "o_exception.h"

struct o_database * o_database_new(char * connection_url)
{
	return o_database_new_error_handler(connection_url, 0);
}

struct o_database * o_database_new_error_handler(char * connection_url, struct o_database_error_handler * error_handler)
{
	struct o_database * new_db = o_malloc(sizeof(struct o_database));
	o_database_new_internal(new_db, connection_url, error_handler);
	return new_db;
}

void o_database_new_internal(struct o_database * db, char * connection_url, struct o_database_error_handler * error_handler)
{
	memset(db, 0, sizeof(struct o_database));
	db->error_handler = error_handler;
	db->connection_url = connection_url;
}

void o_database_reset_error_handler(struct o_database * db, struct o_database_error_handler *error_handler)
{
	db->error_handler = error_handler;
}

void o_database_open(struct o_database * db, char * username, char * password)
{
	try
	{
		db->storage = o_engine_get_storage(db->connection_url, username, password);
	}
	catch(struct o_exception , ex)
	{
		o_database_error_handler_notify(db->error_handler, o_exception_code(ex), o_exception_message(ex));
	}
	end_try;
}

struct o_record_id * o_database_save(struct o_database * db, struct o_record * record)
{
	return o_database_save_cluster(db, record, NULL);
}

struct o_record_id * o_database_save_cluster(struct o_database * db, struct o_record * record, char * cluster_name)
{
	struct o_record_id *rec_id = o_record_get_id(record);
	int is_new = o_record_id_is_new(rec_id);
	struct o_record_id *id;
	struct o_raw_buffer * buff = o_raw_buffer_record(record);
	if (is_new)
	{
		int cluster_id = cluster_name != 0 ? o_storage_get_cluster_id_by_name(db->storage, cluster_name) : o_storage_get_default_cluser_id(
				db->storage);

		long long pos = o_storage_create_record(db->storage, cluster_id, buff);
		id = o_record_id_new(cluster_id, pos);
	}
	else
	{
		id = o_record_get_id(record);
		o_storage_update_record(db->storage, id, buff);
	}
	o_raw_buffer_free(buff);
	return id;
}

void o_database_delete(struct o_database * db, struct o_record * record)
{
	o_storage_delete_record(db->storage, o_record_get_id(record), o_record_version(record));
}

struct o_record * o_database_load(struct o_database * db, struct o_record_id * rid)
{
	struct o_raw_buffer * row = o_storage_read_record(db->storage, rid);
	struct o_record * record = o_record_factory(o_raw_buffer_type(row));
	int size;
	unsigned char * bytes = o_raw_buffer_content(row, &size);
	struct o_input_stream * stream = o_input_stream_new_bytes(bytes, size);
	o_record_deserialize(record, stream);
	o_input_stream_free(stream);
	o_raw_buffer_free(row);
	return record;
}

struct o_record * o_database_record_new()
{
	return o_record_factory(RAW_RECORD_TYPE);
}

void o_database_free_internal(struct o_database * db)
{
	o_database_close(db);
}

void o_database_close(struct o_database * db)
{
	if (db->storage != 0)
		o_storage_release(db->storage);
}

void o_database_free(struct o_database * db)
{
	o_database_free_internal(db);
	o_free(db);
}
