#include "o_storage_internal.h"
#include "o_memory.h"

void o_storage_internal_free(struct o_storage *storage)
{
	o_free(storage->name);
}
