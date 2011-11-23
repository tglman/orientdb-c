#include "test_commons.h"
#include <string.h>
#include <stdio.h>
#include <ctesf.h>

void o_db_error_handler_function(int code, char * error, void * custom_info)
{
	printf("code: message:%s",error);
	fail_args("code: message:%s",error);
}

