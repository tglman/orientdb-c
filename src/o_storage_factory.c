#include "o_storage_factory.h"
#include "o_storage_factory_internal.h"
#include "o_storage_factory_remote.h"
#include "o_memory.h"
#include "o_engine.h"
#include <string.h>
struct o_storage_factory;

struct o_storage_factory * o_storage_factory_new(enum o_url_type type, char * path)
{
	struct o_storage_factory * fact;
	switch (type)
	{
	case REMOTE:
		fact = o_storage_factory_remote_new(path);
		break;
	case LOCAL:
	case HTTP:
	default:
		break;
	}
	fact->path = o_memdup(path, strlen(path));
	return fact;
}

enum o_url_type o_storage_factory_get_type(struct o_storage_factory * factory)
{
	return factory->clazz->get_type(factory);
}

struct o_storage * o_storage_factory_storage_open(struct o_storage_factory * factory, char * db_name, char * user, char * passwd)
{
	return factory->clazz->storage_open(factory, db_name, user, passwd);
}

char * o_storage_factory_get_path(struct o_storage_factory * factory)
{
	return factory->path;
}

void o_storage_factory_release_storage(struct o_storage_factory *factory, struct o_storage * storage)
{
	factory->clazz->storage_release(factory, storage);
}

void o_storage_factory_free(struct o_storage_factory *factory)
{
	o_free(factory->path);
	factory->clazz->free(factory);
}
