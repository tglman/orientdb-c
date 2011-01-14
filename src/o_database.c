#include "o_database.h"
#include "o_database_internal.h"
#include "o_database_error_handler.h"
#include "o_memory.h"
#include <string.h>
#include "o_engine.h"
#include "o_storage.h"
#include "o_url_resolver.h"
#include "o_raw_buffer_record.h"
#include "o_input_stream.h"
#include "o_record_factory.h"
#include "o_record_internal.h"
#include "o_record_raw.h"
#include "o_exceptions.h"
#include "o_exception.h"

#define DB_ERROR_NOTIFY(DB,CODE,ERROR) if(DB->error_handler != 0)o_database_error_handler_notify(DB->error_handler, CODE, ERROR);

__thread struct o_database * context_database = 0;

struct o_database * o_database_context_database()
{
	return context_database;
}

void o_database_context_database_init(struct o_database * to_set)
{
	context_database = to_set;
}

struct o_database * o_database_new(char * connection_url)
{
	return o_database_new_error_handler(connection_url, o_database_error_handler_new_default());
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
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
}

int o_database_save(struct o_database * db, struct o_record * record, struct o_record_id ** rid)
{
	return o_database_save_cluster(db, record, NULL, rid);
}

int o_database_save_cluster(struct o_database * db, struct o_record * record, char * cluster_name, struct o_record_id ** rid)
{
	int result = 0;
	o_database_context_database_init(db);
	try
	{
		o_record_before_save(record);
		struct o_record_id *rec_id = o_record_get_id(record);
		int is_new = o_record_id_is_new(rec_id);
		struct o_raw_buffer * buff = o_raw_buffer_record(record);
		if (is_new)
		{
			if(cluster_name == 0)
				cluster_name = o_record_cluster_name(record);
			int cluster_id;
			if (cluster_name != 0)
				cluster_id = o_storage_get_cluster_id_by_name(db->storage, cluster_name);
			else
				cluster_id = o_storage_get_default_cluser_id(db->storage);

			long long pos = o_storage_create_record(db->storage, cluster_id, buff);
			struct o_record_id * new_rid = o_record_id_new(cluster_id, pos);
			o_record_reset_id(record, new_rid);
			if (rid != 0)
				*rid = new_rid;
		}
		else
		{
			struct o_record_id *id = o_record_get_id(record);
			int new_version = o_storage_update_record(db->storage, id, buff);
			o_record_reset_version(record, new_version);
			if (rid != 0)
				*rid = id;
		}
		o_raw_buffer_free(buff);
		o_record_after_save(record);
		result = 1;
	}
	catch( struct o_exception ,ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
	return result;
}

int o_database_delete(struct o_database * db, struct o_record * record)
{
	int result = 0;
	o_database_context_database_init(db);
	try
	{
		o_storage_delete_record(db->storage, o_record_get_id(record), o_record_version(record));
		result = 1;
	}
	catch( struct o_exception ,ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
	return result;
}

struct o_record * o_database_internal_load_record(struct o_database * db, struct o_record_id * rid, struct o_record * to_fill)
{
	o_database_context_database_init(db);
	try
	{
		struct o_raw_buffer * row = o_storage_read_record(db->storage, rid);
		o_record_id_refer(rid);
		if (to_fill == 0)
			to_fill = o_record_factory_id(o_raw_buffer_type(row), rid);
		o_record_reset_version(to_fill, o_raw_buffer_version(row));
		int size;
		unsigned char * bytes = o_raw_buffer_content(row, &size);
		struct o_input_stream * stream = o_input_stream_new_bytes(bytes, size);
		o_record_deserialize(to_fill, stream);
		o_input_stream_free(stream);
		o_raw_buffer_free(row);
	}
	catch( struct o_exception ,ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
	return to_fill;
}

void o_database_load_record(struct o_database * db, struct o_record * record)
{
	o_database_internal_load_record(db, o_record_get_id(record), record);
}

struct o_record * o_database_load(struct o_database * db, struct o_record_id * rid)
{
	return o_database_internal_load_record(db, rid, 0);
}

struct o_record * o_database_record_new_type(struct o_database *db, char type)
{
	o_database_context_database_init(db);
	struct o_record * new_rec = o_record_factory(type);
	o_database_context_database_init(0);
	return new_rec;
}

struct o_record * o_database_record_new(struct o_database *db)
{
	return o_database_record_new_type(db, RAW_RECORD_TYPE);
}

void o_database_free_internal(struct o_database * db)
{
	o_database_close(db);
}

void o_database_close(struct o_database * db)
{
	try
	{
		if (db->storage != 0)
			o_storage_release(db->storage);
	}
	catch(struct o_exception , ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;

}

void o_database_free(struct o_database * db)
{
	o_database_free_internal(db);
	o_free(db);
}
