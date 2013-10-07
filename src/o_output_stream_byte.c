#include "o_output_stream_internal.h"
#include "o_output_stream_byte.h"
#include "o_memory.h"
#include <string.h>
#define START_BUFFER_SIZE 100

struct o_output_stream_byte
{
	struct o_output_stream stream;
	unsigned char * content;
	int size;
	int cursor;
};

void o_output_stream_byte_write_bytes(struct o_output_stream * stream, void *bytes, int nbytes)
{
	struct o_output_stream_byte *buff = (struct o_output_stream_byte *) stream;
	while (buff->cursor + nbytes >= buff->size)
	{
		buff->size = buff->size * 2;
		buff->content = o_realloc(buff->content, buff->size);
	}
	memcpy(buff->content + buff->cursor, bytes, nbytes);
	buff->cursor += nbytes;
}

void o_output_stream_byte_flush(struct o_output_stream * stream)
{

}

void o_output_stream_byte_free(struct o_output_stream * stream)
{
	struct o_output_stream_byte *stream_byte = (struct o_output_stream_byte *) stream;
	o_free(stream_byte->content);
	o_free(stream_byte);
}

unsigned char * o_output_stream_byte_content(struct o_output_stream * stream, int * content_size)
{
	struct o_output_stream_byte *buff = (struct o_output_stream_byte *) stream;
	*content_size = buff->cursor;
	return buff->content;
}

struct o_output_stream * o_output_stream_byte_buffer()
{
	struct o_output_stream_byte *stream = o_malloc(sizeof(struct o_output_stream_byte));
	stream->stream.type = 'b';
	stream->stream.o_output_stream_flush = o_output_stream_byte_flush;
	stream->stream.o_output_stream_free = o_output_stream_byte_free;
	stream->stream.o_output_stream_write_bytes = o_output_stream_byte_write_bytes;
	stream->content = o_malloc(sizeof(char) * START_BUFFER_SIZE);
	stream->size = START_BUFFER_SIZE;
	stream->cursor = 0;

	return (struct o_output_stream *) stream;
}
