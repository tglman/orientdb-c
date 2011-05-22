#include "o_pool.h"
#include "o_memory.h"
#include "o_list.h"
#include "o_native_lock.h"
#include <string.h>

struct o_pool
{
	int min_size;
	int max_size;
	void * factory_obj;
	void * (*factory)(void * obj);
	void (*free)(void * obj, void * instance);
	struct o_list *free_instances;
	struct o_list *used_instances;
	struct o_native_lock *lock;
};

int o_pool_curret_size(struct o_pool * pool);

void o_pool_check_reduce(struct o_pool * pool)
{
	int used_size = o_list_size(pool->used_instances);
	int free_size = o_list_size(pool->free_instances);
	int total_size = used_size + free_size;
	if (total_size > pool->min_size && free_size / 4 > used_size)
	{
		int to_free = free_size / 2;
		if (total_size - to_free < pool->min_size)
			to_free = total_size - pool->min_size;
		while (to_free >= 0)
		{
			void * inst = o_list_get(pool->free_instances, 0);
			o_list_remove(pool->free_instances, inst);
			pool->free(pool->factory_obj, inst);
			to_free--;
		}
	}
}

int o_pool_expand(struct o_pool * pool)
{
	int pool_size = o_pool_curret_size(pool);
	if (pool_size < pool->max_size)
	{
		int used_size = o_list_size(pool->used_instances);
		if (used_size * 2 > pool->max_size)
			used_size = pool->max_size - used_size;
		while (used_size > 0)
		{
			o_list_add(pool->free_instances, pool->factory(pool->factory_obj));
			used_size--;
		}
		return 1;
	}
	return 0;
}

struct o_pool * o_pool_new_size(void * obj, void * (*factory)(void *), void(*factory_free)(void *, void *), int min_size, int max_size)
{
	struct o_pool * pool = o_malloc(sizeof(struct o_pool));
	memset(pool, 0, sizeof(struct o_pool));
	pool->factory_obj = obj;
	pool->factory = factory;
	pool->free = factory_free;
	pool->min_size = min_size;
	pool->max_size = max_size;
	pool->free_instances = o_list_new();
	pool->used_instances = o_list_new();
	pool->lock = o_native_lock_new();
	return pool;
}

struct o_pool * o_pool_new(void * obj, void * (*factory)(void *), void(*free)(void *, void *))
{
	return o_pool_new_size(obj, factory, free, 1, 10);
}

int o_pool_curret_size(struct o_pool * pool)
{
	return o_list_size(pool->free_instances) + o_list_size(pool->used_instances);
}

void * o_pool_get(struct o_pool * pool)
{
	void * inst = 0;
	o_native_lock_lock(pool->lock);
	if (o_list_size(pool->free_instances) == 0)
		if (!o_pool_expand(pool))
		{
			o_native_lock_unlock(pool->lock);
			return 0;
		}

	inst = o_list_get(pool->free_instances, 0);
	o_list_remove(pool->free_instances, inst);
	o_list_add(pool->used_instances, inst);

	o_native_lock_unlock(pool->lock);
	return inst;
}

void o_pool_release(struct o_pool * pool, void * to_release)
{
	o_native_lock_lock(pool->lock);
	if (o_list_contains(pool->used_instances, to_release))
	{
		o_list_remove(pool->used_instances, to_release);
		o_list_add(pool->free_instances, to_release);
		o_pool_check_reduce(pool);
	}
	o_native_lock_unlock(pool->lock);
}

void o_pool_free(struct o_pool * to_free)
{
	if (o_list_size(to_free->used_instances) > 0)
	{
		//TODO:throw an exception.
	}
	struct o_list_iterator * it = o_list_begin(to_free->free_instances);
	do
	{
		to_free->free(to_free->factory_obj, o_list_iterator_current(it));
	} while (o_list_iterator_next(it));
	o_list_free(to_free->free_instances);
	o_list_free(to_free->used_instances);
	o_native_lock_free(to_free->lock);
	o_free(to_free);
}
