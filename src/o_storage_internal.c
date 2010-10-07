#include "o_connection_internal.h"

void o_storage_internal_free(struct o_storage *storage)
{
	o_free(storage->name);
}
