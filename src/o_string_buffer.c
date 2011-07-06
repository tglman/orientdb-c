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
	int expected_cursor = buff->cursor + size;
	if (expected_cursor >= buff->size)
	{
		buff->size *= 2;
		if (expected_cursor >= buff->size)
			buff->size = expected_cursor;
		buff->buffer = o_realloc(buff->buffer, buff->size);
	}
	memcpy(buff->buffer + buff->cursor, to_append, size);
	buff->cursor += size;
}

void o_string_buffer_append_long(struct o_string_buffer * buff, long long long_val)
{
	char val[20];
	sprintf(val, "%Li", long_val);
	o_string_buffer_append(buff, val);
}

void o_string_buffer_append_double(struct o_string_buffer * buff, double double_val)
{
	char val[20];
	sprintf(val, "%g", double_val);
	o_string_buffer_append(buff, val);
}

void o_string_buffer_append_char(struct o_string_buffer * buff, char ch)
{
	char val[2] =
	{ ch, 0 };
	o_string_buffer_append(buff, val);
}

void o_string_buffer_clear(struct o_string_buffer * buff)
{
	buff->cursor = 0;
}

char * o_string_buffer_str(struct o_string_buffer * buff)
{
	char * new_str = o_malloc(buff->cursor + 1);
	memcpy(new_str, buff->buffer, buff->cursor);
	new_str[buff->cursor] = 0;
	return new_str;
}

int o_string_buffer_size(struct o_string_buffer * buff)
{
	return buff->cursor;
}

void o_string_buffer_free(struct o_string_buffer * to_free)
{
	o_free(to_free->buffer);
	o_free(to_free);
}
