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
struct o_internal_result_handler
{
	struct o_list_record *list;
	struct o_database * db;
};

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
			if (cluster_name == 0)
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
		o_record_cache_put(o_database_get_cache(db), record);
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
		o_record_cache_remove(o_database_get_cache(db), record);
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

struct o_record * o_database_record_from_content(struct o_database * db, struct o_record_id * rid, struct o_raw_buffer * content)
{
	struct o_record * record = 0;
	o_database_context_database_init(db);
	try
	{
		o_record_id_refer(rid);
		record = o_record_factory_id(o_raw_buffer_type(content), rid);
		o_record_reset_version(record, o_raw_buffer_version(content));
		int size;
		unsigned char * bytes = o_raw_buffer_content(content, &size);
		struct o_input_stream * stream = o_input_stream_new_bytes(bytes, size);
		o_record_deserialize(record, stream);
		o_input_stream_free(stream);
		o_raw_buffer_free(content);
		o_record_cache_put(o_database_get_cache(db), record);
	}
	catch( struct o_exception ,ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	return record;
}

struct o_record * o_database_load(struct o_database * db, struct o_record_id * rid)
{
	struct o_record * rec = o_record_cache_get(o_database_get_cache(db), rid);
	if (rec == 0)
	{
		try
		{
			struct o_raw_buffer * row = o_storage_read_record(db->storage, rid);
			rec = o_database_record_from_content(db, rid, row);
		}
		catch( struct o_exception ,ex)
		{
			DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
			o_exception_free(ex);
		}
		end_try;
	}
	return rec;
}

void o_query_engine_record_listener(void * add_info, struct o_record_id *id, struct o_raw_buffer * buffer)
{
	struct o_internal_result_handler *result_handler = (struct o_internal_result_handler *) add_info;
	struct o_record * record = o_database_record_from_content(result_handler->db, id, buffer);
	o_list_record_add(result_handler->list, record);
}

struct o_list_record * o_database_query(struct o_database * db, struct o_query * query)
{
	struct o_internal_result_handler result_handler;
	result_handler.db = db;
	struct o_query_engine * query_engine = o_storage_get_query_engine(db->storage);
	o_query_engine_query(query_engine, query, &result_handler, o_query_engine_record_listener);
	return result_handler.list;
}

struct o_record * o_database_record_new_type(struct o_database *db, char type)
{
	o_database_context_database_init(db);
	struct o_record * new_rec = o_record_factory(type);
	o_database_context_database_init(0);
	return new_rec;
}

struct o_record_cache * o_database_get_cache(struct o_database * db)
{
	if (db->cache == 0)
		db->cache = o_record_cache_new();
	return db->cache;
}

struct o_record * o_database_record_new(struct o_database *db)
{
	return o_database_record_new_type(db, RAW_RECORD_TYPE);
}

void o_database_free_internal(struct o_database * db)
{
	if (db->referrers != 0)
	{
		int count = o_list_size(db->referrers);
		while (count > 0)
			*((struct o_database **) o_list_get(db->referrers, --count)) = 0;
		o_list_free(db->referrers);
	}
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

void o_database_add_referrer(struct o_database * db, struct o_database ** referrer)
{
	if (db->referrers == 0)
		db->referrers = o_list_new();
	o_list_add(db->referrers, referrer);
}

void o_database_remove_referrer(struct o_database * db, struct o_database ** referrer)
{
	if (db->referrers != 0)
		o_list_remove(db->referrers, referrer);
}

struct o_record * o_database_metadata(struct o_database * db)
{
	struct o_raw_buffer * meta = o_storage_get_metadata(db->storage);
	return o_database_record_from_content(db, o_record_id_new(0, 0), meta);
}

void o_database_free(struct o_database * db)
{
	o_database_free_internal(db);
	o_free(db);
}
