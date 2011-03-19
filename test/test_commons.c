#include "test_commons.h"
#include <string.h>
#include <stdio.h>
#include <test.h>

void o_db_error_handler_function(int code, char * error, void * custom_info)
{
	char message[512];
	sprintf(message, "code:%i message:%s", code, error);
	fail(message);
}

