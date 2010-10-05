#include "o_exception_internal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "o_memory.h"

struct o_exception * o_exception_new(char * message, int code)
{
	struct o_exception * new_ex = o_malloc(sizeof(struct o_exception));
	o_exception_internal_init(new_ex, message, code);
	return new_ex;
}

char * o_exception_what_generator(struct o_exception * exception)
{
	int base_size = strlen(exception->message);
	char * what_string = o_malloc(sizeof(char) * (base_size + 50));
	sprintf(what_string, "error:%i:%s", exception->code, exception->message);
	return what_string;
}

void o_exception_default_free(struct o_exception *exception)
{
	o_exception_internal_free(exception);
	o_free(exception);
}

void o_exception_internal_init(struct o_exception * exception, char * message, int code)
{
	int len = strlen(message);
	exception->message = o_malloc(sizeof(char) * len);
	memcpy(exception->message, message, len);
	exception->code = code;
	exception->what_cache = 0;
	exception->what = o_exception_what_generator;
	exception->free = o_exception_default_free;
	exception->match_type = o_exception_match_exception;
}

int o_exception_match_exception(struct o_exception * exception, char * type)
{
	if (strcmp("struct o_exception", type) == 0)
		return 1;
	return 0;
}

int o_exception_typeof(struct o_exception * exception, char * type)
{
	return exception->match_type(exception, type);
}

char * o_exception_message(struct o_exception *exception)
{
	return exception->message;
}

int o_exception_code(struct o_exception *exception)
{
	return exception->code;
}

char * o_exception_what(struct o_exception * exception)
{
	if (exception->what_cache == 0)
	{
		exception->what_cache = exception->what(exception);
	}
	return exception->what_cache;
}

void o_exception_internal_free(struct o_exception * exception)
{
	if (exception->what_cache != 0)
		o_free(exception->what_cache);
	o_free(exception->free);
}
