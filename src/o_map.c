#include "o_map.h"
#include <string.h>
#include "o_memory.h"

#define DEFAULT_ENTRIES_SIZE 10

struct o_map_entry
{
	char * key;
	void * value;
	struct o_map_entry * next;
	struct o_map_entry * before;
	struct o_map_entry * map_next;
	struct o_map_entry * map_before;
};

struct o_map
{
	struct o_map_entry * first;
	struct o_map_entry * last;
	struct o_map_entry **entries;
	int entries_size;
	int size;
	char ** cache_keys;
	void ** cache_values;
};

unsigned int o_map_hash_string(struct o_map * map, char * key)
{
	unsigned int h, i;
	unsigned int len = strlen(key);
	for (h = 0, i = 0; i < len; i++)
		h = 31 * h + key[i++];
	return h % map->entries_size;
}

void o_map_clear_caches(struct o_map *map)
{
	o_free(map->cache_keys);
	map->cache_keys = 0;
	o_free(map->cache_values);
	map->cache_values = 0;
}

struct o_map * o_map_new()
{
	struct o_map * new_map = o_malloc(sizeof(struct o_map));
	memset(new_map, 0, sizeof(struct o_map));
	new_map->entries_size = DEFAULT_ENTRIES_SIZE;
	new_map->entries = o_malloc(sizeof(struct o_map_entry *) * new_map->entries_size);
	memset(new_map->entries, 0, sizeof(struct o_map_entry *) * new_map->entries_size);
	return new_map;
}

void o_map_free_entry(struct o_map_entry * entry)
{
	o_free(entry->key);
	o_free(entry);
}

struct o_map_entry * o_map_get_entry(struct o_map * map, char * key, unsigned int hash)
{
	struct o_map_entry * cur = map->entries[hash];
	while (cur != 0 && strcmp(cur->key, key) != 0)
		cur = cur->next;
	return cur;
}

void * o_map_put(struct o_map * map, char * key, void * val)
{
	unsigned int hash = o_map_hash_string(map, key);
	o_map_clear_caches(map);
	struct o_map_entry * new_entry = o_map_get_entry(map, key, hash);
	if (new_entry != 0)
	{
		void * old_val = new_entry->value;
		new_entry->value = val;
		return old_val;
	}
	new_entry = o_malloc(sizeof(struct o_map_entry));

	new_entry->before = 0;
	new_entry->map_next = 0;
	new_entry->map_before = map->last;
	if (map->last != 0)
		map->last->map_next = new_entry;
	map->last = new_entry;
	if (map->first == 0)
		map->first = new_entry;

	int len_key = strlen(key) + 1;
	char * key_copy = o_malloc(sizeof(char) * len_key);
	memcpy(key_copy, key, len_key);
	new_entry->key = key_copy;
	new_entry->value = val;

	new_entry->next = map->entries[hash];
	if (new_entry->next != 0)
		new_entry->next->before = new_entry;
	map->entries[hash] = new_entry;
	map->size++;
	return 0;
}

void * o_map_get(struct o_map * map, char * key)
{
	unsigned int hash = o_map_hash_string(map, key);
	struct o_map_entry * found = o_map_get_entry(map, key, hash);
	if (found != 0)
		return found->value;
	return 0;
}

void * o_map_remove(struct o_map * map, char * key)
{
	unsigned int hash = o_map_hash_string(map, key);
	struct o_map_entry * found = o_map_get_entry(map, key, hash);
	void *value = 0;
	if (found != 0)
	{
		value = found->value;
		found->map_before->map_next = found->map_next;
		if (found->before != 0)
			found->before->next = found->next;
		else
			map->entries[hash] = found->next;

		o_map_free_entry(found);
	}
	map->size--;
	o_map_clear_caches(map);
	return value;
}

char ** o_map_keys(struct o_map * map, int * keys_num)
{
	if (map->cache_keys == 0)
	{
		int i = 0;
		map->cache_keys = o_malloc(sizeof(char *) * map->size);
		struct o_map_entry *iter = map->first;
		while (iter != 0)
		{
			map->cache_keys[i] = iter->key;
			++i;
			iter = iter->map_next;
		}
	}
	*keys_num = map->size;
	return map->cache_keys;
}

void ** o_map_values(struct o_map * map, int * values_num)
{
	if (map->cache_values == 0)
	{
		int i = 0;
		map->cache_values = o_malloc(sizeof(void *) * map->size);
		struct o_map_entry *iter = map->first;
		while (iter != 0)
		{
			map->cache_values[i] = iter->value;
			++i;
			iter = iter->map_next;
		}

	}
	*values_num = map->size;
	return map->cache_values;
}

int o_map_size(struct o_map * map)
{
	return map->size;
}

void o_map_free(struct o_map * map)
{
	if (map->first != 0)
	{
		struct o_map_entry *iter = map->first->map_next;
		while (iter != 0)
		{
			o_map_free_entry(iter->map_before);
			iter = iter->map_next;
		}
	}
	o_free(map->entries);
	o_free(map);
}
