#include "o_engine.h"
#include "o_memory.h"
#include "o_storage_factory.h"
#include "o_map_string.h"
#include <stdio.h>
#include <string.h>
#include "o_memory.h"

struct o_engine
{
	struct o_map_string * factories;
};

static struct o_engine *global_engine = 0;
void o_engine_release();

struct o_engine * o_engine_get_instance()
{
	if (global_engine == 0)
	{
		global_engine = (struct o_engine *) o_malloc(sizeof(struct o_engine));
		global_engine->factories = o_map_string_new();
	}
	return global_engine;
}

struct o_storage_factory * o_engine_get_factory(enum o_url_type type, char *path)
{
	struct o_engine *cur_engine = o_engine_get_instance();
	struct o_storage_factory * new_con;
	if ((new_con = o_map_string_get(cur_engine->factories, path)) == 0)
	{
		new_con = o_storage_factory_new(type, path);
		o_map_string_put(cur_engine->factories, path, new_con);
	}
	return new_con;
}

struct o_storage * o_engine_get_storage(char * connection_url, char * username, char * password)
{
	enum o_url_type type;
	char *path;
	char * db_name;
	o_url_resolve_information(connection_url, &type, &path, &db_name);
	struct o_storage_factory * conn = o_engine_get_factory(type, path);
	return o_storage_factory_storage_open(conn, db_name, username, password);
}

void o_engine_release_factory(struct o_storage_factory * factory)
{
	 char * path = o_storage_factory_get_path(factory);
	 o_map_string_remove(o_engine_get_instance()->factories, path);
	 if (o_map_string_size(o_engine_get_instance()->factories) == 0)
	 o_engine_release();

}

void o_engine_release()
{
	o_map_string_free(global_engine->factories);
	o_free(global_engine);
	global_engine = 0;
}
