#include "o_input_stream.h"
#include "o_memory.h"

enum input_stream_type
{
	BYTE_ARRAY
};

struct o_input_stream
{
	enum input_stream_type type;
	void * content;
};

struct o_input_stream_bytes
{
	char * bytes;
	int lenght;
	int cursor;
};

struct o_input_stream * o_input_stream_new(enum input_stream_type type, int content_size)
{
	struct o_input_stream *str = o_malloc(sizeof(struct o_input_stream));
	str->type = type;
	str->content = o_malloc(content_size);
	return str;
}

struct o_input_stream * o_input_stream_new_bytes(char * bytes, int lenght)
{

	struct o_input_stream * new_stre = o_input_stream_new(BYTE_ARRAY, sizeof(struct o_input_stream_bytes));
	struct o_input_stream_bytes * bytes_struct = new_stre->content;
	bytes_struct->bytes = bytes;
	bytes_struct->lenght = lenght;
	bytes_struct->cursor = 0;
	return new_stre;
}

int o_input_stream_read(struct o_input_stream * stream)
{
	if (stream->type == BYTE_ARRAY)
	{
		struct o_input_stream_bytes * bytes_struct = stream->content;
		return bytes_struct->bytes[bytes_struct->cursor++];
	}
	return 0;
}

void o_input_stream_free(struct o_input_stream * to_free)
{
	o_free(to_free->content);
	o_free(to_free);
}
