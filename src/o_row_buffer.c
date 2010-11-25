#include "o_row_buffer.h"
#include "o_record.h"
#include "o_row_buffer_internal.h"

int o_raw_buffer_content_size(struct o_raw_buffer * buff)
{
	return buff->o_raw_buffer_content_size(buff);
}

char * o_raw_buffer_content(struct o_raw_buffer * buff, int * content_size)
{
	return buff->o_raw_buffer_content(buff, content_size);
}

int o_raw_buffer_version(struct o_raw_buffer * buff)
{
	return buff->o_raw_buffer_version(buff);
}

char o_raw_buffer_type(struct o_raw_buffer * buff)
{
	return buff->o_raw_buffer_type(buff);
}
