#include "o_map_string.h"
#include "o_map.h"
#include "o_memory.h"
#include <string.h>

struct o_map_string
{
	struct o_map *map;
};

unsigned int o_map_hash_string(void * key, int size)
{
	char * string_key = key;
	unsigned int h, i;
	unsigned int len = strlen(string_key);
	for (h = 0, i = 0; i < len; i++)
		h = 31 * h + string_key[i++];
	return h % size;
}

void o_entry_string_create(void ** key, void ** value)
{
	*key = o_memdup(*key, strlen((char *) *key) + 1);
}

void o_entry_string_free(void ** key, void ** value)
{
	o_free(*key);
}

struct o_map_string * o_map_string_new()
{
	struct o_map_string * string = o_malloc(sizeof(struct o_map_string));
	string->map = o_map_new(o_map_hash_string, o_entry_string_create, o_entry_string_free);
	return string;
}

void * o_map_string_put(struct o_map_string * map, char * key, void * val)
{
	return o_map_put(map->map, key, val);
}

void * o_map_string_remove(struct o_map_string * map, char * key)
{
	return o_map_remove(map->map, key);
}

void * o_map_string_get(struct o_map_string * map, char * key)
{
	return o_map_get(map->map, (void *) key);
}

char ** o_map_string_keys(struct o_map_string * map, int * keys_num)
{
	return (char **) o_map_keys(map->map, keys_num);
}

void ** o_map_string_values(struct o_map_string * map, int * values_num)
{
	return o_map_values(map->map, values_num);
}

int o_map_string_size(struct o_map_string * map)
{
	return o_map_size(map->map);
}

void o_map_string_clear(struct o_map_string * map)
{
	o_map_clear(map->map);
}

void o_map_string_free(struct o_map_string * map)
{
	return o_map_free(map->map);
}

