#include "o_database_operation_context.h"
#include "o_operation_context_internal.h"
#include "o_record_internal.h"
#include "o_record_cache.h"
#include "o_raw_buffer_record.h"
#include "o_record_factory.h"
#include "o_list_record.h"
#include "o_query_engine.h"
#include "o_memory.h"
#include <string.h>

struct o_database_operation_context
{
	struct o_operation_context context;
	struct o_storage * storage;
	struct o_record_cache * cache;
};

struct o_internal_result_handler
{
	QueryHandler handler;
	void * handler_add_info;
	struct o_database_operation_context * db;
};

int o_database_operation_context_cluster_name_id(struct o_operation_context * context, char * cluster_name)
{
	struct o_database_operation_context *db = (struct o_database_operation_context *) context;
	int cluster_id;
	if (cluster_name != 0)
		cluster_id = o_storage_get_cluster_id_by_name(db->storage, cluster_name);
	else
		cluster_id = o_storage_get_default_cluser_id(db->storage);
	return cluster_id;
}

int o_database_operation_context_save(struct o_operation_context * context, struct o_record * record, char * cluster_name, struct o_record_id ** rid)
{
	struct o_database_operation_context *db = (struct o_database_operation_context *) context;
	o_record_before_save(record);
	struct o_record_id *rec_id = o_record_get_id(record);
	int is_new = o_record_id_is_new(rec_id);
	struct o_raw_buffer * buff = o_raw_buffer_record(record);
	if (is_new)
	{
		if (cluster_name == 0)
			cluster_name = o_record_cluster_name(record);
		int cluster_id = o_operation_context_cluster_name_id(context, cluster_name);

		struct create_result * res = o_storage_create_record(db->storage, cluster_id, buff);
		struct o_record_id * new_rid = o_record_id_new(cluster_id, res->rid);
		o_record_reset_id(record, new_rid);
		o_record_reset_version(record, res->version);
		o_free(res);
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
	o_record_cache_put(db->cache, record);
	return 1;
}

int o_database_operation_context_delete(struct o_operation_context * context, struct o_record * record)
{
	struct o_database_operation_context *db = (struct o_database_operation_context *) context;
	o_record_cache_remove(db->cache, record);
	o_storage_delete_record(db->storage, o_record_get_id(record), o_record_version(record));
	return 1;
}

struct o_record * o_database_operation_context_record_from_content(struct o_database_operation_context * db, struct o_record_id * rid,
		struct o_raw_buffer * content)
{
	struct o_record * record = 0;
	o_record_id_refer(rid);
	record = o_record_factory_id(o_raw_buffer_type(content), rid);
	o_record_reset_version(record, o_raw_buffer_version(content));
	int size;
	unsigned char * bytes = o_raw_buffer_content(content, &size);
	struct o_input_stream * stream = o_input_stream_new_bytes(bytes, size);
	o_record_deserialize(record, stream);
	o_input_stream_free(stream);
	o_raw_buffer_free(content);
	o_record_cache_put(db->cache, record);
	return record;
}

struct o_record * o_database_operation_context_load(struct o_operation_context * context, struct o_record_id * record_id)
{
	struct o_database_operation_context *db = (struct o_database_operation_context *) context;
	struct o_record * rec = o_record_cache_get(db->cache, record_id);
	if (rec == 0)
	{
		struct o_raw_buffer * row = o_storage_read_record(db->storage, record_id);
		rec = o_database_operation_context_record_from_content(db, record_id, row);
	}
	return rec;
}

int o_database_operation_context_commit(struct o_operation_context * context)
{
	return 0;
}

int o_database_operation_context_rollback(struct o_operation_context * context)
{
	return 0;
}

int o_database_operation_context_commit_transaction(struct o_operation_context * context, struct o_transaction * transaction)
{
	struct o_database_operation_context *db = (struct o_database_operation_context *) context;
	o_storage_commit_transaction(db->storage, transaction);
	return 0;
}

void o_c_query_engine_record_listener(void * add_info, struct o_record_id *id, struct o_raw_buffer * buffer)
{
	struct o_internal_result_handler *result_handler = (struct o_internal_result_handler *) add_info;
	struct o_record * record = o_database_operation_context_record_from_content(result_handler->db, id, buffer);
	result_handler->handler(result_handler->handler_add_info, record);
}

int o_database_operation_context_query(struct o_operation_context * context, struct o_query * query, void ** parameters, QueryHandler handler,
		void * handler_add_info)
{
	struct o_database_operation_context *db = (struct o_database_operation_context *) context;
	struct o_internal_result_handler result_handler;
	result_handler.db = db;
	result_handler.handler = handler;
	result_handler.handler_add_info = handler_add_info;
	struct o_query_engine * query_engine = o_storage_get_query_engine(db->storage);
	o_query_engine_query(query_engine, query, &result_handler, o_c_query_engine_record_listener);
	return 1;
}

struct o_operation_context * o_database_operation_context_release(struct o_operation_context * context)
{
	struct o_database_operation_context * ctx = (struct o_database_operation_context *) context;
	o_record_cache_free(ctx->cache);
	o_free(ctx);
	return 0;
}

struct o_operation_context_class o_database_operation_context_instance =
{ .save = o_database_operation_context_save, .delete = o_database_operation_context_delete, .load = o_database_operation_context_load, .commit =
		o_database_operation_context_commit, .commit_transaction = o_database_operation_context_commit_transaction, .query = o_database_operation_context_query,
		.release = o_database_operation_context_release, .rollback = o_database_operation_context_rollback, .cluster_name_id =
				o_database_operation_context_cluster_name_id };

struct o_operation_context * o_database_operation_context(struct o_storage * storage)
{
	struct o_database_operation_context * context = (struct o_database_operation_context *) o_malloc(sizeof(struct o_database_operation_context));
	memset(context, 0, sizeof(struct o_database_operation_context));
	context->context.type = &o_database_operation_context_instance;
	context->storage = storage;
	context->cache = o_record_cache_new();
	return &context->context;
}
