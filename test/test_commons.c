#include "suites.h"
#include <string.h>
#include <stdio.h>

void o_db_error_handler_function(int code, char * error, void * custom_info)
{
	printf("code: message:%s",error);
	
	ck_abort_msg("code: message:%s",error);
}

