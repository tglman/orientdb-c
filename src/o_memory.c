#include "o_memory.h"

void * o_malloc(size_t size)
{
	return malloc(size);
}

void o_free(void * ptr)
{
	free(ptr);
}
