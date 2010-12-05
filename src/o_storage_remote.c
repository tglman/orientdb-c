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

#define CLUSTER_INTERNAL_NAME "internal"
#define CLUSTER_INDEX_NAME "index"
#define CLUSTER_DEFAULT_NAME "default"
#define DATA_DEFAULT_NAME "default"

#define CURRENT_VERSION 0;

// COMMANDS
#define CONNECT 1

#define DB_OPEN	 5
#define DB_CREATE 6
#define DB_CLOSE 7
#define DB_EXIST 8

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

void o_storage_remote_begin_response(struct o_storage_remote * storage)
{
	if (o_connection_remote_begin_read_session(storage->connection, storage->session_id) == ERROR)
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
	try
	{
		o_connection_remote_begin_write_session(rs->connection, rs->session_id, RECORD_CREATE);
		o_connection_remote_write_short(rs->connection, cluster);
		int size;
		unsigned char * buff = o_raw_buffer_content(content, &size);
		o_connection_remote_write_bytes(rs->connection, buff, size);
		o_connection_remote_write_byte(rs->connection, o_raw_buffer_type(content));
		o_connection_remote_end_write(rs->connection);
	}
	catch(struct o_exception_io, cur_ex)
	{
		o_exception_free((struct o_exception *) cur_ex);
	}
	end_try;

	o_storage_remote_begin_response(rs);
	long long val = o_connection_remote_read_long64(rs->connection);
	o_connection_remote_end_read(rs->connection);
	o_storage_release_exclusive_lock(rs);
	return val;
}

struct o_raw_buffer * o_storage_remote_read_record(struct o_storage * storage, struct o_record_id * id)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);

	o_connection_remote_begin_write_session(rs->connection, rs->session_id, RECORD_LOAD);
	o_connection_remote_write_short(rs->connection, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(rs->connection, o_record_id_record_id(id));
	o_connection_remote_write_string(rs->connection, "");
	o_connection_remote_end_write(rs->connection);

	o_storage_remote_begin_response(rs);
	char res = o_connection_remote_read_byte(rs->connection);
	if (res == 0)
		return 0;
	int size;
	unsigned char * o_p = o_connection_remote_read_bytes(rs->connection, &size);
	int version = o_connection_remote_read_int(rs->connection);
	int type = o_connection_remote_read_int(rs->connection);
	o_connection_remote_end_read(rs->connection);

	o_storage_release_exclusive_lock(rs);
	return o_raw_buffer_byte(type, version, o_p, size);
}

int o_storage_remote_update_record(struct o_storage * storage, struct o_record_id * id, struct o_raw_buffer * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);

	o_connection_remote_begin_write_session(rs->connection, rs->session_id, RECORD_UPDATE);
	o_connection_remote_write_short(rs->connection, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(rs->connection, o_record_id_record_id(id));
	int size;
	unsigned char* bytes = o_raw_buffer_content(content, &size);
	o_connection_remote_write_bytes(rs->connection, bytes, size);
	o_connection_remote_write_int(rs->connection, o_raw_buffer_version(content));
	o_connection_remote_write_int(rs->connection, o_raw_buffer_type(content));
	o_connection_remote_end_write(rs->connection);

	o_storage_remote_begin_response(rs);
	int res = o_connection_remote_read_int(rs->connection);
	o_connection_remote_end_read(rs->connection);

	o_storage_release_exclusive_lock(rs);
	return res;
}

int o_storage_remote_delete_record(struct o_storage * storage, struct o_record_id * id, int version)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);

	o_connection_remote_begin_write_session(rs->connection, rs->session_id, RECORD_DELETE);
	o_connection_remote_write_short(rs->connection, o_record_id_cluster_id(id));
	o_connection_remote_write_long64(rs->connection, o_record_id_record_id(id));
	o_connection_remote_write_int(rs->connection, version);
	o_connection_remote_end_write(rs->connection);

	o_storage_remote_begin_response(rs);
	int res = o_connection_remote_read_int(rs->connection);
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

struct o_storage * o_storage_remote_new(struct o_connection_remote * conn, char * name, char * username, char * password)
{
	struct o_storage_remote * storage = 0;
	try
	{
		storage = o_malloc(sizeof(struct o_storage_remote));
		storage->storage.name = name;
		storage->storage.o_storage_create_record = o_storage_remote_create_record;
		storage->storage.o_storage_read_record = o_storage_remote_read_record;
		storage->storage.o_storage_update_record = o_storage_remote_update_record;

		storage->storage.o_storage_delete_record = o_storage_remote_delete_record;
		storage->storage.o_storage_get_cluster_names = o_storage_remote_get_cluster_names;
		storage->storage.o_storage_get_cluster_id_by_name = o_storage_remote_get_cluster_id_by_name;

		storage->storage.o_storage_commit_transaction = o_storage_remote_commit_transaction;
		storage->storage.o_storage_free = o_storage_remote_free;
		storage->connection = conn;
		storage->session_id = 0;
		storage->exclusive_lock = o_native_lock_new();

		o_connection_remote_begin_write_session(storage->connection, storage->session_id, DB_OPEN);
		o_connection_remote_write_bytes(storage->connection, (unsigned char*) name, strlen(name));
		o_connection_remote_write_bytes(storage->connection, (unsigned char*) username, strlen(username));
		o_connection_remote_write_bytes(storage->connection, (unsigned char*) password, strlen(password));
		o_connection_remote_end_write(storage->connection);

		o_storage_remote_begin_response(storage);
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
