#include "o_connection_internal.h"
#include "o_database_error_handler.h"
#include "o_storage_internal.h"
#include "o_memory.h"
#include "o_engine.h"
#include <stdlib.h>
#include <string.h>

struct o_connection * o_connection_new(enum o_url_type type, char * path)
{
	struct o_connection *conn = 0;
	int port;
	char * host;
	switch (type)
	{
	case REMOTE:
		if (o_url_resolve_host_port_from_path(path, &host, &port))
			port = 2424;
		conn = o_connection_remote_new(path, port);
		break;
	case HTTP:
		break;
	case LOCAL:
		break;
	}
	conn->path = path;
	conn->type = type;
	conn->storages = o_map_string_new();
	return conn;
}

char * o_connection_build_storage_name(char * name, char * username)
{
	char * store_name = o_malloc(strlen(name) + strlen(username) + 2);
	int len = strlen(name);
	memcpy(store_name, name, len);
	store_name[len] = '\'';
	len += strlen(username);
	memcpy(store_name, username, len);
	store_name[len] = 0;
	return store_name;
}

struct o_storage * o_connection_storage_open(struct o_connection * conn, char * name, char * username, char * password)
{
	char * store_name = o_connection_build_storage_name(name, username);
	struct o_storage * str = o_map_string_get(conn->storages, store_name);
	if (str == 0)
	{
		conn->last_user = username;
		str = conn->storage_open(conn, name, username, password);
		o_map_string_put(conn->storages, store_name, str);
	}
	o_storage_reference(str);
	o_free(store_name);
	return str;
}

char * o_connection_get_last_user(struct o_connection * connection)
{
	return connection->last_user;
}

char * o_connection_get_path(struct o_connection * connection)
{
	return connection->path;
}

void o_connection_storage_release(struct o_connection * conn, struct o_storage * storage)
{
	char * name = o_storage_get_name(storage);
	char * username = o_storage_get_user(storage);
	char * store_name = o_connection_build_storage_name(name, username);
	o_map_string_remove(conn->storages, store_name);
	o_storage_close(storage);
	o_storage_free(storage);
	o_free(store_name);
	if (o_map_string_size(conn->storages) == 0)
		o_engine_release_connection(conn);
}

void o_connection_free(struct o_connection * connection)
{
	if (connection->storages != 0)
		o_map_string_free(connection->storages);
	o_free(connection->path);
	connection->free(connection);
}
