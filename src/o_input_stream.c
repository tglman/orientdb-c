#include "o_input_stream.h"
#include "o_memory.h"
#include "o_input_stream_internal.h"
#include <string.h>

struct o_input_stream_bytes
{
	struct o_input_stream stream;
	unsigned char * bytes;
	int lenght;
	int cursor;
};

int o_input_stream_read(struct o_input_stream * stream)
{
	return stream->o_input_stream_read(stream);
}

int o_input_stream_read_bytes(struct o_input_stream * stream, void * bytes, int size)
{
	return stream->o_input_stream_read_bytes(stream, bytes, size);
}

void o_input_stream_free(struct o_input_stream * to_free)
{
	to_free->o_input_stream_free(to_free);
}

int o_input_stream_peek(struct o_input_stream * stream)
{
	return stream->o_input_stream_peek(stream);
}

int o_input_stream_bytes_read(struct o_input_stream * stream)
{
	struct o_input_stream_bytes * bytes_struct = (struct o_input_stream_bytes *) stream;
	if (bytes_struct->cursor >= bytes_struct->lenght)
		return -1;
	return (int) bytes_struct->bytes[bytes_struct->cursor++];
}

int o_input_stream_bytes_read_bytes(struct o_input_stream * stream, void * bytes, int size)
{
	struct o_input_stream_bytes * bytes_struct = (struct o_input_stream_bytes *) stream;
	if (bytes_struct->cursor >= bytes_struct->lenght)
		return 0;
	if (bytes_struct->cursor + size >= bytes_struct->lenght)
		size = bytes_struct->lenght - (bytes_struct->cursor);
	memcpy(bytes, bytes_struct->bytes + bytes_struct->cursor, size);
	bytes_struct->cursor += size;
	return size;
}

int o_input_stream_bytes_peek(struct o_input_stream * stream)
{
	struct o_input_stream_bytes * bytes_struct = (struct o_input_stream_bytes *) stream;
	if (bytes_struct->cursor >= bytes_struct->lenght)
		return -1;
	return (int) bytes_struct->bytes[bytes_struct->cursor];
}

void o_input_stream_bytes_free(struct o_input_stream * to_free)
{
	o_free(to_free);
}

struct o_input_stream * o_input_stream_new_bytes(unsigned char * bytes, int lenght)
{
	struct o_input_stream_bytes * new_stre = o_malloc(sizeof(struct o_input_stream_bytes));
	new_stre->bytes = bytes;
	new_stre->lenght = lenght;
	new_stre->cursor = 0;
	new_stre->stream.o_input_stream_read = o_input_stream_bytes_read;
	new_stre->stream.o_input_stream_peek = o_input_stream_bytes_peek;
	new_stre->stream.o_input_stream_read_bytes = o_input_stream_bytes_read_bytes;
	new_stre->stream.o_input_stream_free = o_input_stream_bytes_free;
	return &new_stre->stream;
}

