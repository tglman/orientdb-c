#include "o_exceptions.h"
#include <string.h>

__thread jmp_buf exception_try_pos_jmp;

struct exception
{
	char * name;
	void * instance;
};

__thread struct exception cur_exception;

int o_catch_type(char * name)
{
	if (cur_exception.name == 0)
		return o_exception_typeof(cur_exception.instance, name);
	else if (strcmp(cur_exception.name, name) == 0)
		return 1;
	else
		return 0;
}

void * o_catch_value()
{
	return cur_exception.instance;
}

void o_notify_object(char * name, void * instance)
{
	cur_exception.name = name;
	cur_exception.instance = instance;
	longjmp(exception_try_pos_jmp, 1);
}

void o_notify_exception(struct o_exception * exception)
{
	cur_exception.name = 0;
	cur_exception.instance = exception;
	longjmp(exception_try_pos_jmp, 1);
}

