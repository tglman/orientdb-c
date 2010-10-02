#include "o_database.h"
#include "o_database_internal.h"
#include "o_memory.h"
#include <string.h>
#include "o_connection.h"
#include "o_storage.h"
#include "o_url_resolver.h"

struct o_database * o_database_new(char * connection_url)
{
	return o_database_new_error_handler(connection_url, 0);
}

struct o_database * o_database_new_error_handler(char * connection_url, struct o_database_error_handler * error_handler)
{
	struct o_database * new_db = o_malloc(sizeof(struct o_database));
	o_database_new_internal(new_db, connection_url, error_handler);
	return new_db;
}

void o_database_new_internal(struct o_database * db, char * connection_url, struct o_database_error_handler * error_handler)
{
	memset(db, 0, sizeof(struct o_database));
	db->error_handler = error_handler;
}

void o_database_reset_error_handler(struct o_database * db, struct o_database_error_handler *error_handler)
{
	db->error_handler = error_handler;
}

void o_database_open(struct o_database * db, char * username, char * password)
{
	enum o_url_type type;
	char *path;
	char * db_name;
	o_url_resolve_information(db->connection_url, &type, &path, &db_name, db->error_handler);
	db->connection = o_connection_new(type, path, username, password, db->error_handler);
	db->storage = o_connection_storage_open(db->connection, db_name);
}

void o_database_free_internal(struct o_database * db)
{
	if (db->storage != 0)
		o_storage_free(db->storage);
	if (db->connection != 0)
		o_connection_free(db->connection);
}

void o_database_free(struct o_database * db)
{
	o_database_free_internal(db);
	o_free(db);
}
