#include "o_engine.h"
#include "o_memory.h"
#include "o_connection.h"
#include "o_map.h"

struct o_engine
{
	int count;
	struct o_map * connections;
};

static struct o_engine *global_engine = 0;

struct o_engine * o_engine_get_instance()
{
	if (global_engine == 0)
	{
		global_engine = (struct o_engine *) o_malloc(sizeof(struct o_engine));
		global_engine->count = 1;
		global_engine->connections = o_map_new();
	}
	else
		global_engine->count++;
	return global_engine;
}

struct o_connection * o_engine_get_connection(enum o_url_type type, char *path)
{
	struct o_connection * new_con;
	if ((new_con = o_map_get(global_engine->connections, path)) == 0)
	{
		new_con = o_connection_new(type, path);
		o_map_put(global_engine->connections, path, new_con);
	}

	return new_con;
}

struct o_storage * o_engine_get_storage(char * connection_url, char * username, char * password)
{
	enum o_url_type type;
	char *path;
	char * db_name;
	o_url_resolve_information(connection_url, &type, &path, &db_name);
	struct o_connection * conn = o_engine_get_connection(type, path);
	return o_connection_storage_open(conn, db_name, username, password);
}

void o_engine_release()
{
	global_engine->count--;
	if (global_engine->count == 0)
	{
		int sizes;
		void ** values = o_map_values(global_engine->connections, &sizes);
		while (sizes > 0)
			o_connection_free(values[sizes--]);
		o_map_free(global_engine->connections);
		o_free(global_engine);
		global_engine = 0;
	}
}
