#include "o_connection_remote.h"
#include "o_connection_internal.h"
#include "socket_interface.h"
#include "o_storage_remote.h"
#include "o_memory.h"
#include <stdlib.h>
#include <string.h>

struct o_connection_remote
{
	//Inerith
	struct o_connection connection;
	struct o_database_socket *socket;
};

void o_connection_remote_free(struct o_connection *connection);
struct o_storage * o_connection_remote_storage_open(struct o_connection *connection, char * name,char * username, char * password);

struct o_connection * o_connection_remote_new(char * host, int port)
{
	struct o_connection_remote * conn = o_malloc(sizeof(struct o_connection_remote));
	memset(conn, 0, sizeof(struct o_connection_remote));
	conn->socket = o_database_socket_connect(host, port);
	conn->connection.free = o_connection_remote_free;
	return &conn->connection;
}

struct o_storage * o_connection_remote_storage_open(struct o_connection *connection, char * name,char * username, char * password)
{
	//TODO: ask username and password validation
	struct o_connection_remote * remote = (struct o_connection_remote *) connection;
	struct o_storage * new_storage = o_storage_remote_new(remote);
	return new_storage;
}

int o_connection_remote_read_int(struct o_connection_remote * connection)
{
	//TODO:
	return 0;
}

long long o_connection_remote_read_long64(struct o_connection_remote * connection)
{
	//TODO:
	return 0;
}

char o_connection_remote_read_byte(struct o_connection_remote * connection)
{
	//TODO:
	return 0;
}

short o_connection_remote_read_short(struct o_connection_remote * connection)
{
	//TODO:
	return 0;
}

char * o_connection_remote_read_bytes(struct o_connection_remote * connection, int *byte_read)
{
	//TODO:
	return 0;
}

char ** o_connection_remote_read_array_strings(struct o_connection_remote * connection, int *array_size)
{
	//TODO:
	return 0;
}

void o_connection_remote_write_int(struct o_connection_remote * connection, int int_value)
{
	//TODO:
}

void o_connection_remote_write_long64(struct o_connection_remote * connection, long long long_value)
{
	//TODO:
}

void o_connection_remote_write_byte(struct o_connection_remote * connection, char byte_value)
{
	//TODO:
}

void o_connection_remote_write_short(struct o_connection_remote * connection, short short_value)
{
	//TODO:
}

void o_connection_remote_write_bytes(struct o_connection_remote * connection, char *byte_array, int length)
{
	//TODO:
}

void o_connection_remote_flush(struct o_connection_remote * connection)
{
	//TODO:
}

void o_connection_remote_write_array_strings(struct o_connection_remote * connection, char * *strings_array, int length)
{
	//TODO:
}

void o_connection_remote_free(struct o_connection *connection)
{
	struct o_connection_remote * remote = (struct o_connection_remote *) connection;
	o_database_socket_close(remote->socket);
	o_free(remote);
}

