#include "o_database_error_handler.h"
#include "o_memory.h"
#include <stdio.h>

struct o_database_error_handler
{
	o_database_error_handler_function error_handler;
	void * custom_info;
};

void o_database_error_handler_default_function(int code, char * error, void * custom_info)
{
	fprintf(stderr, "%i:%s", code, error);
	fflush(stderr);
}

struct o_database_error_handler * o_database_error_handler_new(o_database_error_handler_function function, void * custom_info)
{
	struct o_database_error_handler * new_error_handler = o_malloc(sizeof(struct o_database_error_handler));

	new_error_handler->error_handler = function;
	new_error_handler->custom_info = custom_info;
	return new_error_handler;
}

struct o_database_error_handler * o_database_error_handler_new_default()
{
	return o_database_error_handler_new(o_database_error_handler_default_function, 0);
}

void o_database_error_handler_notify(struct o_database_error_handler * error_handler, int code, char * error)
{
	error_handler->error_handler(code, error, error_handler->custom_info);
}

void o_database_error_handler_free(struct o_database_error_handler * error_handler)
{
	o_free(error_handler);
}
