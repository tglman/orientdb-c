#include "o_memory.h"

void * o_malloc(size_t size)
{
	return malloc(size);
}

void * o_realloc(void * cur, int new_size)
{
	return realloc(cur, new_size);
}

void o_free(void * ptr)
{
	free(ptr);
}
