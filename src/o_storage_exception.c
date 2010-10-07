#include "o_storage_exception.h"
#include "o_memory.h"
#include "o_exception_internal.h"
#include <string.h>

struct o_storage_exception
{
	struct o_exception base;
};

int o_storage_exception_match(struct o_exception * ex, char *type)
{
	if (strcmp("struct o_storage_exception", type) == 0)
		return 1;
	else
		return o_exception_match_exception(ex, type);
}

void o_storage_exception_free(struct o_exception * ex)
{
	o_exception_internal_free(ex);
	struct o_storage_exception * ex_st = (struct o_storage_exception *) ex;
	o_free(ex_st);
}

struct o_storage_exception * o_storage_exception_new(char *message, int code)
{
	struct o_storage_exception * new_ex = o_malloc(sizeof(struct o_storage_exception));
	o_exception_internal_init(&new_ex->base, message, code);
	new_ex->base.match_type = o_storage_exception_match;
	new_ex->base.free = o_storage_exception_free;
	return new_ex;
}
