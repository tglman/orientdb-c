#include "o_memory.h"
#include <string.h>

void * o_malloc(size_t size)
{
	return malloc(size);
}

void * o_realloc(void * cur, size_t new_size)
{
	return realloc(cur, new_size);
}

void * o_memdup(void * to_dup, size_t dup_len)
{
	void * new_mem = o_malloc(dup_len);
	memcpy(new_mem, to_dup, dup_len);
	return new_mem;
}

void o_free(void * ptr)
{
	free(ptr);
}
