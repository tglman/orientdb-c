#include "o_storage_remote.h"
#include "o_storage_internal.h"
#include "o_exceptions.h"
#include "o_exception_io.h"
#include <stdlib.h>
#include <string.h>
#include "o_memory.h"
#include "o_connection_remote.h"

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
	char * sessionId;
	int n_cluster;
	struct o_storage_remote_cluster *clusters;
};

void o_storage_acquire_exclusive_lock(struct o_storage_remote * storage)
{

}

void o_storage_release_exclusive_lock(struct o_storage_remote * storage)
{

}

void o_storage_remote_check_status(struct o_storage_remote * storage)
{
	if (o_connection_remote_read_byte(storage->connection) == ERROR)
	{

	}
}

long long o_storage_remote_create_record(struct o_storage * storage, int cluster, struct o_raw_buffer * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);
	try
	{
		o_connection_remote_write_byte(rs->connection, RECORD_CREATE);
		o_connection_remote_write_short(rs->connection, cluster);
		int size;
		unsigned char * buff = o_raw_buffer_content(content, &size);
		o_connection_remote_write_bytes(rs->connection, buff, size);
		o_connection_remote_write_byte(rs->connection, o_raw_buffer_type(content));
		o_connection_remote_flush(rs->connection);
	}
	catch(struct o_exception_io, cur_ex)
	{
		o_exception_free((struct o_exception *) cur_ex);
	}
	end_try;
	o_storage_release_exclusive_lock(rs);
	//readStatus();
	//return network.readLong();
	return 0;
}

struct o_raw_buffer * o_storage_remote_read_record(struct o_storage * storage, struct o_record_id * id)
{
	return 0;
}

int o_storage_remote_update_record(struct o_storage * storage, struct o_record_id * id, struct o_raw_buffer * content)
{
	return 0;
}

int o_storage_remote_delete_record(struct o_storage * storage, struct o_record_id * id, int version)
{
	return 0;
}

char ** o_storage_remote_get_cluster_names(struct o_storage * storage, int * names_count)
{
	return 0;
}

int o_storage_remote_get_cluster_id_by_name(struct o_storage * storage, char * name)
{
	return 0;
}

void o_storage_remote_commit_transaction(struct o_storage *storage, struct o_transaction * transaction)
{

}

void o_storage_remote_free(struct o_storage * storage)
{
	o_storage_internal_free(storage);
	struct o_storage_remote * storage_remote = (struct o_storage_remote *) storage;
	o_free(storage_remote->sessionId);
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

		o_connection_remote_write_byte(storage->connection, DB_OPEN);
		o_connection_remote_write_bytes(storage->connection, (unsigned char*) name, strlen(name));
		o_connection_remote_write_bytes(storage->connection, (unsigned char*) username, strlen(username));
		o_connection_remote_write_bytes(storage->connection, (unsigned char*) password, strlen(password));
		o_connection_remote_flush(storage->connection);
		o_storage_remote_check_status(storage);
		int readSize;
		storage->sessionId = o_connection_remote_read_bytes(storage->connection, &readSize);
		storage->n_cluster = o_connection_remote_read_int(storage->connection);
		int i;
		struct o_storage_remote_cluster **clusters = &storage->clusters;
		for (i = 0; i < storage->n_cluster; i++)
		{
			struct o_storage_remote_cluster *cluster = o_malloc(sizeof(struct o_storage_remote_cluster));
			cluster->storage_name = o_connection_remote_read_bytes(storage->connection, &readSize);
			cluster->id = o_connection_remote_read_int(storage->connection);
			cluster->type = o_connection_remote_read_bytes(storage->connection, &readSize);
			*clusters = cluster;
			clusters = &cluster->next;
		}
		//MANAGE DEFAULT CLUSTER.
	}
	catch(struct o_exception, ex)
	{
		o_free(storage);
		throw(ex);
	}
	end_try;

	return &storage->storage;
}
