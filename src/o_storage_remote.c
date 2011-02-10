#include "o_storage_remote.h"
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
#include <time.h>
#include <stdio.h>

// COMMANDS
#define SHUTDOWN 1
#define CONNECT 2

#define DB_OPEN	 4
#define DB_CREATE 5
#define DB_CLOSE 6
#define DB_EXIST 7
#define DB_DELETE 8
#define DB_SIZE 9

#define CLUSTER_ADD 10
#define CLUSTER_REMOVE 11
#define CLUSTER_COUNT 12

#define DATASEGMENT_ADD 20
#define DATASEGMENT_REMOVE 21

#define RECORD_LOAD 30
#define RECORD_CREATE 31
#define RECORD_UPDATE 32
#define RECORD_DELETE 33

#define COUNT 40
#define COMMAND 41

#define DICTIONARY_LOOKUP 50
#define DICTIONARY_PUT 51
#define DICTIONARY_REMOVE 52
#define DICTIONARY_SIZE 53
#define DICTIONARY_KEYS 54

#define TX_COMMIT 100

// STATUSES
#define OK	 0
#define ERROR 1

// CONSTANTS
#define RECORD_NULL -2

int o_storage_remote_new_request_id()
{
	static int cur_id = 0;
	return (cur_id++) + time(0);
}

struct o_storage_remote_cluster
{
	char * storage_name;
	int id;
	char * type;
	struct o_storage_remote_cluster *next;
};

struct o_storage_remote
{
	struct o_storage storage;
	struct o_connection_remote * connection;
	struct o_native_lock * exclusive_lock;
	int session_id;
	int n_cluster;
	struct o_storage_remote_cluster *clusters;
	int default_cluster_id;
};

void o_storage_acquire_exclusive_lock(struct o_storage_remote * storage)
{
	o_native_lock_lock(storage->exclusive_lock);
}

void o_storage_release_exclusive_lock(struct o_storage_remote * storage)
{
	o_native_lock_unlock(storage->exclusive_lock);
}

void o_storage_remote_begin_response(struct o_storage_remote * storage, int req_id)
{
	fflush(stdout);
	if (o_connection_remote_begin_read_session(storage->connection, req_id) == ERROR)
	{
		short start = 1;
		struct o_string_buffer * buff = o_string_buffer_new();
		while (o_connection_remote_read_byte(storage->connection) == 1)
		{
			char * ex_name = o_connection_remote_read_string(storage->connection);
			char * ex_msg = o_connection_remote_read_string(storage->connection);
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
		char * messg = o_string_buffer_str(buff);
		throw(o_exception_new(messg,30));
		o_string_buffer_free(buff);
	}
}

long long o_storage_remote_create_record(struct o_storage * storage, int cluster, struct o_raw_buffer * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);

	int req_id = o_storage_remote_new_request_id();
	o_connection_remote_begin_write_session(rs->connection, req_id, RECORD_CREATE);
	o_connection_remote_write_short(rs->connection, cluster);
	int size;
	unsigned char * buff = o_raw_buffer_content(content, &size);
	o_connection_remote_write_bytes(rs->connection, buff, size);
	o_connection_remote_write_byte(rs->connection, o_raw_buffer_type(content));
	o_connection_remote_end_write(rs->connection);

	o_storage_remote_begin_response(rs, req_id);
	long long val = o_connection_remote_read_long64(rs->connection);
	o_connection_remote_end_read(rs->connection);
	o_storage_release_exclusive_lock(rs);
	return val;
}

struct o_raw_buffer * o_storage_remote_read_record(struct o_storage * storage, struct o_record_id * id)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);

	int req_id = o_storage_remote_new_request_id();
	o_connection_remote_begin_write_session(rs->connection, req_id, RECORD_LOAD);
	o_connection_remote_write_short(rs->connection, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(rs->connection, o_record_id_record_id(id));
	o_connection_remote_write_string(rs->connection, "");
	o_connection_remote_end_write(rs->connection);

	o_storage_remote_begin_response(rs, req_id);
	char res = o_connection_remote_read_byte(rs->connection);
	if (res == 0)
		return 0;
	int size;
	unsigned char * o_p = o_connection_remote_read_bytes(rs->connection, &size);
	int version = o_connection_remote_read_int(rs->connection);
	char type = o_connection_remote_read_byte(rs->connection);
	//?????? Protocol .....
	o_connection_remote_read_byte(rs->connection);
	o_connection_remote_end_read(rs->connection);

	o_storage_release_exclusive_lock(rs);
	return o_raw_buffer_byte(type, version, o_p, size);
}

int o_storage_remote_update_record(struct o_storage * storage, struct o_record_id * id, struct o_raw_buffer * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);
	int req_id = o_storage_remote_new_request_id();
	o_connection_remote_begin_write_session(rs->connection, req_id, RECORD_UPDATE);
	o_connection_remote_write_short(rs->connection, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(rs->connection, o_record_id_record_id(id));
	int size;
	unsigned char* bytes = o_raw_buffer_content(content, &size);
	o_connection_remote_write_bytes(rs->connection, bytes, size);
	o_connection_remote_write_int(rs->connection, o_raw_buffer_version(content));
	o_connection_remote_write_byte(rs->connection, o_raw_buffer_type(content));
	o_connection_remote_end_write(rs->connection);

	o_storage_remote_begin_response(rs, req_id);
	int res = o_connection_remote_read_int(rs->connection);
	o_connection_remote_end_read(rs->connection);

	o_storage_release_exclusive_lock(rs);
	return res;
}

int o_storage_remote_delete_record(struct o_storage * storage, struct o_record_id * id, int version)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);

	int req_id = o_storage_remote_new_request_id();
	o_connection_remote_begin_write_session(rs->connection, req_id, RECORD_DELETE);
	o_connection_remote_write_short(rs->connection, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(rs->connection, o_record_id_record_id(id));
	o_connection_remote_write_int(rs->connection, version);
	o_connection_remote_end_write(rs->connection);

	o_storage_remote_begin_response(rs, req_id);
	char res = o_connection_remote_read_byte(rs->connection);
	o_connection_remote_end_read(rs->connection);
	o_storage_release_exclusive_lock(rs);
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
	o_connection_storage_release((struct o_connection *) storage_remote->connection, storage);
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
	int req_id = o_storage_remote_new_request_id();
	o_connection_remote_begin_write_session(rs->connection, req_id, DB_CLOSE);
	o_connection_remote_end_write(rs->connection);
}

int o_storage_remote_get_default_cluster_id(struct o_storage * storage)
{
	struct o_storage_remote * rs = (struct o_storage_remote *) storage;
	return rs->default_cluster_id;
}

struct o_storage * o_storage_remote_new(struct o_connection_remote * conn, char * name, char * username, char * password)
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

		storage->storage.o_storage_commit_transaction = o_storage_remote_commit_transaction;

		storage->storage.o_storage_final_release = o_storage_remote_internal_release;
		storage->storage.o_storage_get_metadata = o_storage_remote_get_metadata;
		storage->storage.o_storage_close = o_storage_remote_close;
		storage->storage.o_storage_free = o_storage_remote_free;

		storage->connection = conn;
		storage->session_id = 0;
		storage->exclusive_lock = o_native_lock_new();

		int req_id = o_storage_remote_new_request_id();
		o_connection_remote_begin_write_session(storage->connection, req_id, DB_OPEN);
		o_connection_remote_write_string(storage->connection, name);
		o_connection_remote_write_string(storage->connection, username);
		o_connection_remote_write_string(storage->connection, password);
		o_connection_remote_end_write(storage->connection);

		o_storage_remote_begin_response(storage, req_id);
		storage->session_id = o_connection_remote_read_int(storage->connection);
		storage->n_cluster = o_connection_remote_read_int(storage->connection);
		int i;
		struct o_storage_remote_cluster **clusters = &storage->clusters;
		for (i = 0; i < storage->n_cluster; i++)
		{
			struct o_storage_remote_cluster *cluster = o_malloc(sizeof(struct o_storage_remote_cluster));
			cluster->storage_name = o_connection_remote_read_string(storage->connection);
			cluster->id = o_connection_remote_read_int(storage->connection);
			cluster->type = o_connection_remote_read_string(storage->connection);
			*clusters = cluster;
			clusters = &cluster->next;
			cluster->next = 0;
		}
		int size;
		unsigned char * content = o_connection_remote_read_bytes(storage->connection, &size);
		if (content != 0)
			o_free(content);
		o_connection_remote_end_read(storage->connection);
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
