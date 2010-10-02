#include "o_storage_remote.h"
#include "o_storage_internal.h"
#include "o_exceptions.h"
#include "o_exception_io.h"
#include <stdlib.h>
#include "o_memory.h"

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

struct o_storage_remote
{
	struct o_storage storage;
	struct o_connection_remote * connection;
};

void o_storage_acquire_exclusive_lock(struct o_storage_remote * storage)
{

}

void o_storage_release_exclusive_lock(struct o_storage_remote * storage)
{

}

long long o_storage_remote_create_record(struct o_storage * storage, int cluster, struct o_record_content * content)
{
	struct o_storage_remote *rs = (struct o_storage_remote *) storage;
	o_storage_acquire_exclusive_lock(rs);
	try
	{
		o_connection_remote_write_byte(rs->connection, RECORD_CREATE);
		o_connection_remote_write_short(rs->connection, cluster);
		o_connection_remote_write_bytes(rs->connection, o_record_content_bytes(content), o_record_content_bytes_length(content));
		o_connection_remote_write_byte(rs->connection, o_record_content_type(content));
		o_connection_remote_flush(rs->connection);
	}
	catch(struct o_exception_io, cur_ex)
	{
	}
	o_storage_release_exclusive_lock(rs);
	//readStatus();
	//return network.readLong();
	return 0;
}
struct o_record_content * o_storage_remote_read_record(struct o_storage * storage, struct o_record_id * id, int * version)
{
	return 0;
}

int o_storage_remote_update_record(struct o_storage * storage, struct o_record_id * id, int version, struct o_record_content * content)
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

}

struct o_storage * o_storage_remote_new(struct o_connection_remote * conn, struct o_database_error_handler * error_handler)
{
	struct o_storage_remote * storage = o_malloc(sizeof(struct o_storage_remote));

	storage->storage.o_storage_create_record = o_storage_remote_create_record;
	storage->storage.o_storage_read_record = o_storage_remote_read_record;
	storage->storage.o_storage_update_record = o_storage_remote_update_record;

	storage->storage.o_storage_delete_record = o_storage_remote_delete_record;
	storage->storage.o_storage_get_cluster_names = o_storage_remote_get_cluster_names;
	storage->storage.o_storage_get_cluster_id_by_name = o_storage_remote_get_cluster_id_by_name;

	storage->storage.o_storage_commit_transaction = o_storage_remote_commit_transaction;
	storage->storage.o_storage_free = o_storage_remote_free;
	storage->connection = conn;

	return &storage->storage;
}
