#include "o_output_stream_internal.h"

void o_output_stream_write(struct o_output_stream * stream, char c)
{
	stream->o_output_stream_write(stream, c);
}

void o_output_stream_write_bytes(struct o_output_stream * stream, void *bytes, int nbytes)
{
	stream->o_output_stream_write_bytes(stream, bytes, nbytes);
}

void o_output_stream_flush(struct o_output_stream * stream)
{
	stream->o_output_stream_flush(stream);
}

void o_output_stream_free(struct o_output_stream * stream)
{
	stream->o_output_stream_free(stream);
}
