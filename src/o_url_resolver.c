#include "o_url_resolver.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "o_database_errors.h"

enum o_url_type o_url_resolve_type(char * connection_url, struct o_database_error_handler * error_handler)
{
	char protocol[10];
	strncpy(connection_url, protocol, 9);
	protocol[9] = 0;
	int i;
	for (i = 0; i < 10; i++)
		protocol[i] = toupper(protocol[i]);
	if (strncmp(protocol, HTTP_PROTOCOL_PARAMETER, HTTP_PROTOCOL_PARAMETER_LENGHT) == 0)
		return HTTP;
	else if (strncmp(protocol, REMOTE_PROTOCOL_PARAMETER, REMOTE_PROTOCOL_PARAMETER_LENGHT) == 0)
		return REMOTE;
	else if (strncmp(protocol, LOCAL_PROTOCOL_PARAMETER, LOCAL_PROTOCOL_PARAMETER_LENGHT) == 0)
		return LOCAL;
	else
		return 0;

}

int o_url_resolve_information(char * connection_url, enum o_url_type *type, char **path, char ** db_name,
		struct o_database_error_handler * error_handler)
{
	int pos;
	*type = o_url_resolve_type(connection_url, error_handler);
	switch (*type)
	{
	case HTTP:
		connection_url += HTTP_PROTOCOL_PARAMETER_LENGHT;
		pos = strcspn(connection_url, "/");
		*path = malloc(pos * sizeof(char));
		strncpy(*path, connection_url, pos);
		connection_url += pos + 1;
		pos = strcspn(connection_url, "/");
		connection_url += pos + 1;
		pos = strcspn(connection_url, "/");
		*db_name = malloc(pos * sizeof(char));
		strncpy(*db_name, connection_url, pos);
		break;
	case REMOTE:
		connection_url += HTTP_PROTOCOL_PARAMETER_LENGHT;
		pos = strcspn(connection_url, "/");
		*path = malloc(pos * sizeof(char));
		strncpy(*path, connection_url, pos);
		connection_url += pos + 1;
		pos = strcspn(connection_url, "/");
		*db_name = malloc(pos * sizeof(char));
		strncpy(*db_name, connection_url, pos);
		break;
	case LOCAL:
		connection_url += HTTP_PROTOCOL_PARAMETER_LENGHT;
		*path = strdup(connection_url);
		break;
	default:
	{
		char * message = malloc(sizeof(char) * (strlen(connection_url) + strlen(BAD_CONNECTION_URL_MESSAGE)));
		sprintf(message, BAD_CONNECTION_URL_MESSAGE, connection_url);
		o_database_error_handler_notify(error_handler, BAD_CONNECTION_URL_ID, message);
		free(message);
		return 0;
	}
		break;
	}
	return 1;
}

int o_url_resolve_host_port_from_path(char * path, char ** host, int * port, struct o_database_error_handler * error_handler)
{
	int pos = strcspn(path, ":");
	if (pos != -1)
	{
		*host = malloc(pos * sizeof(char));
		strncpy(*host, path, pos);
		*port = atoi(path + pos);
		return 0;
	}
	else
	{
		*host = strdup(path);
		return 1;
	}
	//return 0;
}
