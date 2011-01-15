#include "o_record_cache.h"
#include "o_memory.h"
#include "o_map.h"

struct o_record_cache
{
	struct o_map * map;
};

unsigned int o_record_id_hash(void *par, int size)
{
	return 0;
}

void * o_record_id_dup(void * dup)
{
	return dup;
}

void o_record_cache_id_free(void * dup)
{
}


struct o_record_cache * o_record_cache_new()
{
	struct o_record_cache * cache = o_malloc(sizeof(struct o_record_cache));
	cache->map = o_map_new(o_record_id_hash, o_record_id_dup,o_record_cache_id_free);
	return cache;
}

void o_record_cache_put(struct o_record_cache * cache, struct o_record * record)
{
	o_record_refer(record);
	o_map_put(cache->map, o_record_get_id(record), record);
}

struct o_record * o_record_cache_get(struct o_record_cache * cache, struct o_record_id *id)
{
	return (struct o_record *) o_map_get(cache->map, id);
}
