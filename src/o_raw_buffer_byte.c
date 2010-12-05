#include "o_raw_buffer_byte.h"
#include "o_raw_buffer_internal.h"
#include "o_memory.h"

struct o_raw_buffer_byte
{
	struct o_raw_buffer buffer;
	int type;
	int version;
	unsigned char * content;
	int content_size;
};

int o_raw_buffer_byte_content_size(struct o_raw_buffer *buff)
{
	return ((struct o_raw_buffer_byte *) buff)->content_size;
}

unsigned char * o_raw_buffer_byte_content(struct o_raw_buffer *buff, int *content_size)
{
	*content_size = ((struct o_raw_buffer_byte *) buff)->content_size;
	return ((struct o_raw_buffer_byte *) buff)->content;
}

int o_raw_buffer_byte_version(struct o_raw_buffer *buff)
{
	return ((struct o_raw_buffer_byte *) buff)->version;
}

char o_raw_buffer_byte_type(struct o_raw_buffer *buff)
{
	return ((struct o_raw_buffer_byte *) buff)->type;
}

void o_raw_buffer_byte_free(struct o_raw_buffer *buff)
{
	o_free(((struct o_raw_buffer_byte *) buff)->content);
	o_free(buff);
}

struct o_raw_buffer * o_raw_buffer_byte(int type, int version, unsigned char * content, int content_size)
{
	struct o_raw_buffer_byte * row_buff = o_malloc(sizeof(struct o_raw_buffer_byte));
	row_buff->type = type;
	row_buff->version = version;
	row_buff->content = content;
	row_buff->content_size = content_size;
	row_buff->buffer.o_raw_buffer_content_size = o_raw_buffer_byte_content_size;
	row_buff->buffer.o_raw_buffer_content = o_raw_buffer_byte_content;
	row_buff->buffer.o_raw_buffer_version = o_raw_buffer_byte_version;
	row_buff->buffer.o_raw_buffer_type = o_raw_buffer_byte_type;
	row_buff->buffer.o_raw_buffer_free = o_raw_buffer_byte_free;
	return (struct o_raw_buffer *) row_buff;

}

