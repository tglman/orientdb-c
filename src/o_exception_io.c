#include "o_exception_io.h"
#include "o_exception_internal.h"
#include <stdlib.h>
#include <string.h>

struct o_exception_io
{
	struct o_exception base;
};

int o_exception_io_match(struct o_exception * ex, char *type)
{
	if (strcmp("struct o_exception_io", type) == 0)
		return 1;
	else
		return o_exception_match_exception(ex, type);
}

void o_exception_io_free(struct o_exception * ex)
{
	o_exception_internal_free(ex);
	struct o_exception_io * ex_io = (struct o_exception_io *)ex;
	free(ex_io);
}

struct o_exception_io *o_exception_io_new(char * message, int code)
{
	struct o_exception_io * new_ex = malloc(sizeof(struct o_exception_io));
	o_exception_internal_init(&new_ex->base, message, code);
	new_ex->base.match_type=o_exception_io_match;
	new_ex->base.free =o_exception_io_free;
	return new_ex;
}
