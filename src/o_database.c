#include "o_database.h"
#include "o_database_internal.h"
#include "o_database_error_handler.h"
#include "o_memory.h"
#include <string.h>
#include <stdio.h>
#include "o_engine.h"
#include "o_storage.h"
#include "o_url_resolver.h"
#include "o_record_raw.h"
#include "o_record_factory.h"
#include "o_record_internal.h"
#include "o_exceptions.h"
#include "o_exception.h"
#include "o_database_operation_context.h"
#include "o_transaction.h"

#define DB_ERROR_NOTIFY(DB,CODE,ERROR) if(DB->error_handler != 0)o_database_error_handler_notify(DB->error_handler, CODE, ERROR);

__thread struct o_database * context_database = 0;

struct o_db_internal_result_handler
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
	o_database_new_internal(new_db, connection_url, error_handler, RAW_DB_TYPE);
	return new_db;
}

void o_database_new_internal(struct o_database * db, char * connection_url, struct o_database_error_handler * error_handler, char db_type)
{
	memset(db, 0, sizeof(struct o_database));
	db->error_handler = error_handler;
	db->connection_url = connection_url;
	db->db_type = db_type;
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
		db->context = o_database_operation_context(db->storage);
	}
	catch(struct o_exception, ex)
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
		result = o_operation_context_save(db->context, record, cluster_name, rid);
	}
	catch( struct o_exception, ex)
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
		result = o_operation_context_delete(db->context, record);
	}
	catch( struct o_exception, ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
	return result;
}

struct o_record * o_database_load(struct o_database * db, struct o_record_id * rid)
{
	struct o_record * rec = 0;
	try
	{
		rec = o_operation_context_load(db->context, rid);
	}
	catch( struct o_exception, ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	return rec;
}

void o_query_context_listener(void * add_info, struct o_record *record)
{
	struct o_db_internal_result_handler *result_handler = (struct o_db_internal_result_handler *) add_info;
	o_list_record_add(result_handler->list, record);
}

int o_database_query_internal(struct o_database * db, struct o_query * query, void ** parameters, QueryHandler callback, void *add_info)
{
	return o_operation_context_query(db->context, query, 0, callback, add_info);
}

struct o_list_record * o_database_query(struct o_database * db, struct o_query * query)
{
	o_database_context_database_init(db);
	try
	{
		struct o_db_internal_result_handler handler;
		handler.list = o_list_record_new();
		if (o_database_query_internal(db, query, 0, o_query_context_listener, &handler))
		{
			return handler.list;
		}
	}
	catch( struct o_exception, ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
	return 0;
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

void o_database_begin_transaction(struct o_database * db)
{
	o_database_context_database_init(db);
	try
	{
		struct o_transaction * new_trans = o_transaction_new(db->context);
		db->context = o_transaction_to_operation_context(new_trans);
	}
	catch( struct o_exception, ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
}

void o_database_commit(struct o_database * db)
{
	o_database_context_database_init(db);
	try
	{
		o_operation_context_commit(db->context);
		db->context = o_operation_context_release(db->context);
	}
	catch( struct o_exception, ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);

}

void o_database_rollback(struct o_database * db)
{
	o_database_context_database_init(db);
	try
	{
		o_operation_context_rollback(db->context);
		db->context = o_operation_context_release(db->context);
	}
	catch( struct o_exception, ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
}

void o_database_free_internal(struct o_database * db)
{
	o_database_error_handler_free(db->error_handler);
}

void o_database_close(struct o_database * db)
{
	try
	{
		if (db->referrers != 0)
		{
			int count = o_list_size(db->referrers);
			while (count > 0)
				*((struct o_database **) o_list_get(db->referrers, --count)) = 0;
			o_list_free(db->referrers);
			db->referrers=0;
		}
		if (db->storage != 0)
			o_storage_close(db->storage);
		if (db->context != 0)
		{
			o_database_operation_context_release(db->context);
		}
	}
	catch(struct o_exception, ex)
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

char o_database_get_type(struct o_database *db)
{
	return db->db_type;
}

struct o_record * o_database_metadata(struct o_database * db)
{
	o_database_context_database_init(db);
	struct o_record * metadata = 0;
	try
	{
		struct o_record_id * rid = o_storage_get_metadata_rid(db->storage);
		metadata = o_database_load(db, rid);
	}
	catch( struct o_exception, ex)
	{
		DB_ERROR_NOTIFY(db, o_exception_code(ex), o_exception_message(ex));
		o_exception_free(ex);
	}
	end_try;
	o_database_context_database_init(0);
	return metadata;
}

void o_database_free(struct o_database * db)
{
	o_database_free_internal(db);
	o_free(db);
}
