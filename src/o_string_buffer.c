#include "o_string_buffer.h"
#include "o_memory.h"
#include <string.h>
#include <stdio.h>

#define START_BUFFER_SIZE 30

struct o_string_buffer
{
	char * buffer;
	int size;
	int cursor;
};

struct o_string_buffer * o_string_buffer_new()
{
	struct o_string_buffer * new_buff = o_malloc(sizeof(struct o_string_buffer));
	memset(new_buff, 0, sizeof(struct o_string_buffer));
	new_buff->buffer = o_malloc(sizeof(char) * START_BUFFER_SIZE);
	new_buff->size = START_BUFFER_SIZE;
	new_buff->cursor = 0;
	return new_buff;
}

void o_string_buffer_append(struct o_string_buffer * buff, char * to_append)
{
	int size = strlen(to_append);
	if (buff->cursor + size >= buff->size)
	{
		buff->size = buff->size * 2;
		buff->buffer = o_realloc(buff->buffer, buff->size);
	}
	memcpy(buff->buffer + buff->cursor, to_append, size);
	buff->cursor += size;
}

void o_string_buffer_append_long(struct o_string_buffer * buff, long long_val)
{
	char val[20];
	sprinf(val,"%i",long_val);
	o_string_buffer_append(buff,val);
}

void o_string_buffer_append_double(struct o_string_buffer * buff, double double_val)
{
	char val[20];
	sprinf(val,"%d",double_val);
	o_string_buffer_append(buff,val);
}

char * o_string_buffer_str(struct o_string_buffer * buff)
{
	char * new_str = o_malloc(buff->cursor + 1);
	memcpy(new_str, buff->buffer, buff->cursor);
	new_str[buff->cursor] = 0;
	return new_str;
}

void o_string_buffer_free(struct o_string_buffer * to_free)
{
	o_free(to_free->buffer);
	o_free(to_free);
}
