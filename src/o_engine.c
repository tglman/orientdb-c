#include "o_engine.h"
#include "o_memory.h"
#include "o_connection.h"
#include "o_map_string.h"
#include <stdio.h>
#include <string.h>
#include "o_memory.h"

struct o_engine
{
	struct o_map_string * connections;
};

static struct o_engine *global_engine = 0;
void o_engine_release();

struct o_engine * o_engine_get_instance()
{
	if (global_engine == 0)
	{
		global_engine = (struct o_engine *) o_malloc(sizeof(struct o_engine));
		global_engine->connections = o_map_string_new();
	}
	return global_engine;
}

char * createKey(char * path, char * username)
{
	int pathLen = strlen(path);
	int usrLen = strlen(username);
	char * val = o_malloc(pathLen + pathLen + 1);
	memcpy(val, path, pathLen);
	memcpy(val + pathLen, username, usrLen);
	val[pathLen + pathLen] = 0;
	return val;
}

struct o_connection * o_engine_get_connection(enum o_url_type type, char *path, char * username)
{
	struct o_engine *cur_engine = o_engine_get_instance();
	struct o_connection * new_con;
	char * key = createKey(path, username);
	if ((new_con = o_map_string_get(cur_engine->connections, key)) == 0)
	{
		new_con = o_connection_new(type, path);
		o_map_string_put(cur_engine->connections, key, new_con);
	}
	o_free(key);
	return new_con;
}

struct o_storage * o_engine_get_storage(char * connection_url, char * username, char * password)
{
	enum o_url_type type;
	char *path;
	char * db_name;
	o_url_resolve_information(connection_url, &type, &path, &db_name);
	struct o_connection * conn = o_engine_get_connection(type, path, username);
	return o_connection_storage_open(conn, db_name, username, password);
}

void o_engine_release_connection(struct o_connection * connection)
{
	char * name = o_connection_get_path(connection);
	char * username = o_connection_get_last_user(connection);
	char * key = createKey(name, username);
	o_map_string_remove(o_engine_get_instance()->connections, key);
	if (o_map_string_size(o_engine_get_instance()->connections) == 0)
		o_engine_release();
	o_free(key);
}

void o_engine_release()
{
	o_map_string_free(global_engine->connections);
	o_free(global_engine);
	global_engine = 0;
}
