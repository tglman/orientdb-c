#include "o_storage_remote_internal.h"
#include "o_storage_internal.h"
#include "o_exceptions.h"
#include "o_exception_io.h"
#include <stdlib.h>
#include <string.h>
#include "o_memory.h"
#include "o_connection_remote.h"
#include "o_string_buffer.h"
#include "o_native_lock.h"
#include "o_raw_buffer_byte.h"
#include "o_storage_configuration.h"
#include "o_remote_protocol_specs.h"
#include "o_query_engine_remote.h"
#include "o_storage_factory_internal.h"
#include "o_transaction_internal.h"
#include "o_record_internal.h"
#include <time.h>
#include <stdio.h>

void o_storage_acquire_exclusive_lock(struct o_storage_remote * storage)
{
	o_native_lock_lock(storage->exclusive_lock);
}

void o_storage_release_exclusive_lock(struct o_storage_remote * storage)
{
	o_native_lock_unlock(storage->exclusive_lock);
}

void o_storage_remote_end_write(struct o_storage_remote * storage, struct o_connection_remote * conn)
{
	o_storage_factory_remote_end_write(storage->storage_factory, conn);
}

struct o_connection_remote * o_storage_remote_begin_write(struct o_storage_remote * storage, char command)
{
	struct o_connection_remote * connection = o_storage_factory_remote_begin_write(storage->storage_factory);
	o_connection_remote_write_byte(connection, command);
	o_connection_remote_write_int(connection, storage->session_id);
	return connection;
}

void o_storage_remote_end_read(struct o_storage_remote * storage, struct o_connection_remote * conn)
{
	o_storage_factory_remote_end_read(storage->storage_factory, conn);
}

struct o_connection_remote * o_storage_remote_begin_response(struct o_storage_remote * storage)
{
	int returnCode;
	struct o_connection_remote * conn = o_storage_factory_remote_begin_read(storage->storage_factory, storage->session_id, &returnCode);
	if (returnCode == ERROR)
	{
		short start = 1;
		struct o_string_buffer * buff = o_string_buffer_new();
		while (o_connection_remote_read_byte(conn) == 1)
		{
			char * ex_name = o_connection_remote_read_string(conn);
			char * ex_msg = o_connection_remote_read_string(conn);
			if (!start)
			{
				o_string_buffer_append(buff, "\n -> ");
			}
			else
				start = 0;
			o_string_buffer_append(buff, ex_name);
			o_string_buffer_append(buff, ":");
			o_string_buffer_append(buff, ex_msg);
			o_free(ex_name);
			o_free(ex_msg);

		}
		o_storage_remote_end_read(storage, conn);
		char * messg = o_string_buffer_str(buff);
		o_string_buffer_free(buff);
		throw(o_exception_new(messg,30));
	}
	return conn;
}

struct create_result * o_storage_remote_create_record(struct o_storage * storage, int cluster, struct o_raw_buffer * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, RECORD_CREATE);
	o_connection_remote_write_int(conn, 0);
	o_connection_remote_write_short(conn, cluster);
	int size;
	unsigned char * buff = o_raw_buffer_content(content, &size);
	o_connection_remote_write_bytes(conn, buff, size);
	o_connection_remote_write_byte(conn, o_raw_buffer_type(content));
	o_connection_remote_write_byte(conn, 0);
	o_storage_remote_end_write(rs, conn);

	struct create_result * res = o_malloc(sizeof(struct create_result));
	conn = o_storage_remote_begin_response(rs);
	res->rid = o_connection_remote_read_long64(conn);
	res->version = o_connection_remote_read_int(conn);
	o_storage_remote_end_read(rs, conn);
	return res;
}

struct o_raw_buffer * o_storage_remote_build_matadata_record(struct o_storage_remote * storage)
{
	struct o_storage_configuration *conf = o_storage_get_configuration((struct o_storage *) storage);
	struct o_string_buffer * buff = o_string_buffer_new();
	struct o_record_id *sid = o_storage_configuration_get_schema(conf);
	if (sid != 0)
	{
		o_string_buffer_append(buff, "schema:#");
		o_string_buffer_append(buff, o_record_id_string(sid));
	}
	char * content = o_string_buffer_str(buff);
	o_string_buffer_free(buff);
	fflush(stdout);
	return o_raw_buffer_byte('d', 0, (unsigned char *) content, strlen(content));

}
__thread int in_get_metadata = 1;
struct o_raw_buffer * o_storage_remote_read_record(struct o_storage * storage, struct o_record_id * id)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	if (o_record_id_cluster_id(id) == 0 && o_record_id_record_id(id) == 0 && in_get_metadata)
	{
		in_get_metadata = 0;
		struct o_raw_buffer * res = o_storage_remote_build_matadata_record(rs);
		in_get_metadata = 1;
		return res;
	}
	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, RECORD_LOAD);
	o_connection_remote_write_short(conn, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(conn, o_record_id_record_id(id));
	o_connection_remote_write_string(conn, "");
	o_connection_remote_write_byte(conn, 0); //IGNORE CACHE
	o_connection_remote_write_byte(conn, 0); //TOMBSTONES FALSE
	o_storage_remote_end_write(rs, conn);

	conn = o_storage_remote_begin_response(rs);
	char res = o_connection_remote_read_byte(conn);
	if (res == 0)
	{
		//TODO:WRONG !!! CLOSEEE
		return 0;
	}
	int size;
	unsigned char * o_p = o_connection_remote_read_bytes(conn, &size);
	int version = o_connection_remote_read_int(conn);
	char type = o_connection_remote_read_byte(conn);
	//?????? Protocol .....
	o_connection_remote_read_byte(conn);
	o_storage_remote_end_read(rs, conn);
	return o_raw_buffer_byte(type, version, o_p, size);
}

int o_storage_remote_update_record(struct o_storage * storage, struct o_record_id * id, struct o_raw_buffer * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;

	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, RECORD_UPDATE);
	o_connection_remote_write_short(conn, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(conn, o_record_id_record_id(id));
	int size;
	unsigned char* bytes = o_raw_buffer_content(content, &size);
	o_connection_remote_write_bytes(conn, bytes, size);
	o_connection_remote_write_int(conn, o_raw_buffer_version(content));
	o_connection_remote_write_byte(conn, o_raw_buffer_type(content));
	o_connection_remote_write_byte(conn, 0);
	o_storage_remote_end_write(rs, conn);

	conn = o_storage_remote_begin_response(rs);
	int res = o_connection_remote_read_int(conn);
	o_storage_remote_end_read(rs, conn);

	return res;
}

int o_storage_remote_delete_record(struct o_storage * storage, struct o_record_id * id, int version)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;

	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, RECORD_DELETE);
	o_connection_remote_write_short(conn, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(conn, o_record_id_record_id(id));
	o_connection_remote_write_int(conn, version);
	o_connection_remote_write_byte(conn, 0);
	o_storage_remote_end_write(rs, conn);

	conn = o_storage_remote_begin_response(rs);
	char res = o_connection_remote_read_byte(conn);
	o_storage_remote_end_read(rs, conn);
	return res == 1;
}

char ** o_storage_remote_get_cluster_names(struct o_storage * storage, int * names_count)
{
	return 0;
}

int o_storage_remote_get_cluster_id_by_name(struct o_storage * storage, char * name)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	struct o_storage_remote_cluster * clusters_iterator = rs->clusters;
	while (clusters_iterator != 0)
	{
		if (strcmp(clusters_iterator->storage_name, name) == 0)
			return clusters_iterator->id;
		clusters_iterator = clusters_iterator->next;
	}
	return -1;
}

const char * o_storage_remote_get_cluster_name_by_id(struct o_storage * storage, int cluster_id)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	struct o_storage_remote_cluster * clusters_iterator = rs->clusters;
	while (clusters_iterator != 0)
	{
		if (clusters_iterator->id == cluster_id)
			return clusters_iterator->storage_name;
		clusters_iterator = clusters_iterator->next;
	}
	return "";
}

void o_storage_remote_commit_transaction(struct o_storage *storage, struct o_transaction * transaction)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, TX_COMMIT);
	//TODO:gen transaction id.
	o_connection_remote_write_int(conn, 1);
	o_connection_remote_write_byte(conn, 1);
	int size = 0;
	struct o_transaction_entry ** entries = o_transaction_get_entries(transaction, &size);
	while (size-- > 0)
	{
		struct o_raw_buffer * buff = o_transaction_entry_get_raw_buffer(entries[size]);
		struct o_record_id * rid = o_transaction_entry_get_rid(entries[size]);
		enum transaction_operation_type type = o_transaction_entry_get_type(entries[size]);
		if (type != SAVE && type != REMOVE)
			continue;
		o_connection_remote_write_byte(conn, 1);
		if (type == SAVE)
		{
			if (o_record_id_is_new(rid))
				o_connection_remote_write_byte(conn, COMMIT_CREATE);
			else
				o_connection_remote_write_byte(conn, COMMIT_UPDATE);
		}
		else if (!o_record_id_is_new(rid))
			o_connection_remote_write_byte(conn, COMMIT_DELETED);

		o_connection_remote_write_short(conn, o_record_id_cluster_id(rid));
		o_connection_remote_write_long64(conn, o_record_id_record_id(rid));
		o_connection_remote_write_byte(conn, o_raw_buffer_type(buff));

		if (type == SAVE)
		{
			if (!o_record_id_is_new(rid))
				o_connection_remote_write_int(conn, o_raw_buffer_version(buff));
			int buff_size = 0;
			unsigned char* bytes = o_raw_buffer_content(buff, &buff_size);
			o_connection_remote_write_bytes(conn, bytes, buff_size);
		}
		else
			o_connection_remote_write_int(conn, o_raw_buffer_version(buff));
		o_raw_buffer_free(buff);

	}
	o_connection_remote_write_byte(conn, 0);
	//NOW SEND NO INDEX UPDATE.
	o_connection_remote_write_int(conn, 0);

	o_storage_remote_end_write(rs, conn);

	conn = o_storage_remote_begin_response(rs);
	int created_records = o_connection_remote_read_int(conn);
	struct o_record_id * old = 0;
	struct o_record_id * new = 0;
	int i;
	for (i = 0; i < created_records; i++)
	{
		int old_cl = o_connection_remote_read_short(conn);
		long long old_id = o_connection_remote_read_long64(conn);
		old = o_record_id_new(old_cl, old_id);
		int new_cl = o_connection_remote_read_short(conn);
		long long new_id = o_connection_remote_read_long64(conn);
		new = o_record_id_new(new_cl, new_id);
		o_transaction_update_id(transaction, old, new);
		o_record_id_release(old);
	}
	int updated_records = o_connection_remote_read_int(conn);
	//struct o_operation_context * ctx = o_transaction_to_operation_context(transaction);
	struct o_record_id * upd = 0;
	for (i = 0; i < updated_records; ++i)
	{
		int cl = o_connection_remote_read_short(conn);
		long long id = o_connection_remote_read_long64(conn);
		int new_version = o_connection_remote_read_int(conn);
		upd = o_record_id_new(cl, id);
		struct o_record * rec = o_transaction_get_record(transaction, upd);
		o_record_id_release(upd);
		//struct o_record * rec = o_operation_context_load(ctx, upd);
		if (rec != 0)
			o_record_reset_version(rec, new_version);
	}
	o_storage_remote_end_read(rs, conn);
}

struct o_record_id * o_storage_remote_get_metadata_rid(struct o_storage * storage)
{
	return o_record_id_new(0, 0);
}

void o_storage_remote_free(struct o_storage * storage)
{
	o_storage_internal_free(storage);
	struct o_storage_remote * storage_remote = (struct o_storage_remote *) storage;
	o_native_lock_free(storage_remote->exclusive_lock);
	struct o_storage_remote_cluster * clusters_iterator = storage_remote->clusters;
	struct o_storage_remote_cluster * next_iterator;
	while (clusters_iterator != 0)
	{
		next_iterator = clusters_iterator->next;
		o_free(clusters_iterator->storage_name);
		o_free(clusters_iterator->type);
		o_free(clusters_iterator);
		clusters_iterator = next_iterator;
	}
	o_free(storage_remote);
}

void o_storage_remote_close(struct o_storage * storage)
{
	struct o_storage_remote * rs = (struct o_storage_remote *) storage;
	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, DB_CLOSE);
	o_storage_remote_end_write(rs, conn);

	struct o_storage_remote * storage_remote = (struct o_storage_remote *) storage;
	o_storage_factory_release_storage((struct o_storage_factory *) storage_remote->storage_factory, storage);
}

int o_storage_remote_get_default_cluster_id(struct o_storage * storage)
{
	struct o_storage_remote * rs = (struct o_storage_remote *) storage;
	return rs->default_cluster_id;
}

struct o_query_engine * o_storage_remote_get_query_engine(struct o_storage * storage)
{
	return o_query_engine_remote_new((struct o_storage_remote *) storage);
}

struct o_storage * o_storage_remote_new(struct o_storage_factory_remote * storage_factory, char * name, char * username, char * password)
{
	struct o_storage_remote * storage = 0;
	try
	{
		storage = o_malloc(sizeof(struct o_storage_remote));
		o_storage_internal_new(&storage->storage, name, username);
		storage->storage.o_storage_create_record = o_storage_remote_create_record;
		storage->storage.o_storage_read_record = o_storage_remote_read_record;
		storage->storage.o_storage_update_record = o_storage_remote_update_record;

		storage->storage.o_storage_delete_record = o_storage_remote_delete_record;
		storage->storage.o_storage_get_cluster_names = o_storage_remote_get_cluster_names;
		storage->storage.o_storage_get_cluster_id_by_name = o_storage_remote_get_cluster_id_by_name;
		storage->storage.o_storage_get_default_cluster_id = o_storage_remote_get_default_cluster_id;
		storage->storage.o_storage_get_query_engine = o_storage_remote_get_query_engine;
		storage->storage.o_storage_commit_transaction = o_storage_remote_commit_transaction;

		storage->storage.o_storage_get_metadata_rid = o_storage_remote_get_metadata_rid;
		storage->storage.o_storage_close = o_storage_remote_close;
		storage->storage.o_storage_free = o_storage_remote_free;
		storage->storage.configuration = 0;
		storage->storage_factory = storage_factory;

		storage->session_id = -1;
		storage->exclusive_lock = o_native_lock_new();

		struct o_connection_remote * conn = o_storage_remote_begin_write(storage, DB_OPEN);
		o_connection_remote_write_string(conn, "orientdb-c client");
		o_connection_remote_write_string(conn, "alpha");
		o_connection_remote_write_short(conn, CURRENT_PROTOCOL);
		//TODO:insert client identifier.
		o_connection_remote_write_string(conn, "TODO");
		o_connection_remote_write_string(conn, name);
		o_connection_remote_write_string(conn, "Document");
		o_connection_remote_write_string(conn, username);
		o_connection_remote_write_string(conn, password);
		o_storage_remote_end_write(storage, conn);

		conn = o_storage_remote_begin_response(storage);
		storage->session_id = o_connection_remote_read_int(conn);
		storage->n_cluster = o_connection_remote_read_short(conn);
		int i;
		struct o_storage_remote_cluster **clusters = &storage->clusters;
		for (i = 0; i < storage->n_cluster; i++)
		{
			struct o_storage_remote_cluster *cluster = o_malloc(sizeof(struct o_storage_remote_cluster));
			cluster->storage_name = o_connection_remote_read_string(conn);
			cluster->id = o_connection_remote_read_short(conn);
			cluster->type = o_connection_remote_read_string(conn);
			cluster->dataSegmentId = o_connection_remote_read_short(conn);
			*clusters = cluster;
			clusters = &cluster->next;
			cluster->next = 0;
		}
		int size;
		unsigned char * content = o_connection_remote_read_bytes(conn, &size);
		if (content != 0)
			o_free(content);
		o_storage_remote_end_read(storage, conn);
		storage->default_cluster_id = o_storage_remote_get_cluster_id_by_name(&storage->storage, CLUSTER_DEFAULT_NAME);
		char * version_build = o_connection_remote_read_string(conn);
		o_free(version_build);
	}
	catch(struct o_exception, ex)
	{
		o_free(storage);
		throw(ex);
	}
	end_try;

	return &storage->storage;
}
