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

long long o_storage_remote_create_record(struct o_storage * storage, int cluster, struct o_raw_buffer * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, RECORD_CREATE);
	o_connection_remote_write_short(conn, cluster);
	int size;
	unsigned char * buff = o_raw_buffer_content(content, &size);
	o_connection_remote_write_bytes(conn, buff, size);
	o_connection_remote_write_byte(conn, o_raw_buffer_type(content));
	o_storage_remote_end_write(rs, conn);

	conn = o_storage_remote_begin_response(rs);
	long long val = o_connection_remote_read_long64(conn);
	o_storage_remote_end_read(rs, conn);
	return val;
}

struct o_raw_buffer * o_storage_remote_read_record(struct o_storage * storage, struct o_record_id * id)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;

	struct o_connection_remote * conn = o_storage_remote_begin_write(rs, RECORD_LOAD);
	o_connection_remote_write_short(conn, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(conn, o_record_id_record_id(id));
	o_connection_remote_write_string(conn, "");
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

void o_storage_remote_commit_transaction(struct o_storage *storage, struct o_transaction * transaction)
{

}

void o_storage_remote_internal_release(struct o_storage * storage)
{
	struct o_storage_remote * storage_remote = (struct o_storage_remote *) storage;
	o_storage_factory_release_storage((struct o_storage_factory *) storage_remote->storage_factory, storage);
}

struct o_raw_buffer * o_storage_remote_get_metadata(struct o_storage * storage)
{
	struct o_storage_configuration *conf = o_storage_get_configuration(storage);
	struct o_string_buffer * buff = o_string_buffer_new();
	o_string_buffer_append(buff, "schema:#");
	o_string_buffer_append(buff, o_record_id_string(o_storage_configuration_get_schema(conf)));
	char * content = o_string_buffer_str(buff);
	o_string_buffer_free(buff);
	fflush(stdout);
	return o_raw_buffer_byte('d', 0, (unsigned char *) content, strlen(content));
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

		storage->storage.o_storage_final_release = o_storage_remote_internal_release;
		storage->storage.o_storage_get_metadata = o_storage_remote_get_metadata;
		storage->storage.o_storage_close = o_storage_remote_close;
		storage->storage.o_storage_free = o_storage_remote_free;
		storage->storage_factory=storage_factory;

		storage->session_id = -1;
		storage->exclusive_lock = o_native_lock_new();

		struct o_connection_remote * conn = o_storage_remote_begin_write(storage, DB_OPEN);
		o_connection_remote_write_string(conn, name);
		o_connection_remote_write_string(conn, username);
		o_connection_remote_write_string(conn, password);
		o_storage_remote_end_write(storage, conn);

		conn = o_storage_remote_begin_response(storage);
		storage->session_id = o_connection_remote_read_int(conn);
		storage->n_cluster = o_connection_remote_read_int(conn);
		int i;
		struct o_storage_remote_cluster **clusters = &storage->clusters;
		for (i = 0; i < storage->n_cluster; i++)
		{
			struct o_storage_remote_cluster *cluster = o_malloc(sizeof(struct o_storage_remote_cluster));
			cluster->storage_name = o_connection_remote_read_string(conn);
			cluster->id = o_connection_remote_read_int(conn);
			cluster->type = o_connection_remote_read_string(conn);
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
	}
	catch(struct o_exception, ex)
	{
		o_free(storage);
		throw(ex);
	}
	end_try;

	return &storage->storage;
}
