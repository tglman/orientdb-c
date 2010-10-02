#include "o_connection_internal.h"
#include "o_database_error_handler.h"
#include "o_storage_internal.h"
#include <stdlib.h>
#include <string.h>

struct o_connection * o_connection_new(enum o_url_type type, char * path, char * username, char * password,
		struct o_database_error_handler * error_handler)
{
	struct o_connection *conn;
	int port;
	char * host;
	switch (type)
	{
	case REMOTE:
		if (!o_url_resolve_host_port_from_path(path, &host, &port, error_handler))
			port = 2424;
		conn = o_connection_remote_new(path, port, username, password, error_handler);
		break;
	case HTTP:
		break;
	case LOCAL:
		break;
	}
	conn->error_handler = error_handler;
	conn->type = type;
	return conn;
}

struct o_storage * o_connection_storage_open(struct o_connection * conn, char * name)
{
	return conn->storage_open(conn, name);
}

void o_connection_free(struct o_connection * connection)
{
	connection->free(connection);
}
