#include "o_output_stream_data.h"
#include "o_output_stream_internal.h"
#include "o_memory.h"
#include <string.h>

struct o_output_stream_data
{
	struct o_output_stream base;
	struct o_output_stream *wrapped;
};

void o_output_stream_date_write_bytes(struct o_output_stream * stream, void *bytes, int nbytes)
{
	struct o_output_stream_data * data = (struct o_output_stream_data *) stream;
	o_output_stream_write_bytes(data->wrapped, bytes, nbytes);
}

void o_output_stream_data_flush(struct o_output_stream * stream)
{
	struct o_output_stream_data * data = (struct o_output_stream_data *) stream;
	o_output_stream_flush(data->wrapped);
}

void o_output_stream_data_free(struct o_output_stream_data * stream)
{
	o_output_stream_data_flush(stream);
	o_free(stream);
}

void o_output_stream_data_internal_free(struct o_output_stream* stream)
{
	o_output_stream_data_free((struct o_output_stream_data *) stream);
}

void o_output_stream_data_write_string(struct o_output_stream_data *data_stream, char * string)
{
	int len = strlen(string);
	o_output_stream_data_write_int(data_stream, len);
	o_output_stream_write_bytes(data_stream->wrapped, string, len);
}

void o_output_stream_data_write_byte(struct o_output_stream_data *data_stream, char bvalue)
{
	o_output_stream_write_bytes(data_stream->wrapped, &bvalue, 1);
}

void o_output_stream_data_write_bool(struct o_output_stream_data *data_stream, int bvalue)
{
	unsigned char c = bvalue != 0 ? 1 : 0;
	o_output_stream_write_bytes(data_stream->wrapped, &c, 1);
}

void o_output_stream_data_write_int(struct o_output_stream_data *data_stream, int ivalue)
{
	char bt[4];
	bt[0] = (ivalue >> 24) & 0xFF;
	bt[1] = (ivalue >> 16) & 0xFF;
	bt[2] = (ivalue >> 8) & 0xFF;
	bt[3] = (ivalue >> 0) & 0xFF;
	o_output_stream_write_bytes(data_stream->wrapped, bt, 4);
}

void o_output_stream_data_write_long(struct o_output_stream_data *data_stream, long long lvalue)
{
	char bt[8];
	bt[0] = (lvalue >> 56) & 0xFF;
	bt[1] = (lvalue >> 48) & 0xFF;
	bt[2] = (lvalue >> 40) & 0xFF;
	bt[3] = (lvalue >> 32) & 0xFF;
	bt[4] = (lvalue >> 24) & 0xFF;
	bt[5] = (lvalue >> 16) & 0xFF;
	bt[6] = (lvalue >> 8) & 0xFF;
	bt[7] = (lvalue >> 0) & 0xFF;
	o_output_stream_write_bytes(data_stream->wrapped, bt, 8);
}

void o_output_stream_data_write_short(struct o_output_stream_data *data_stream, short svalue)
{
	char bt[8];
	bt[0] = (svalue >> 8) & 0xFF;
	bt[1] = (svalue >> 0) & 0xFF;
	o_output_stream_write_bytes(data_stream->wrapped, bt, 2);
}

struct o_output_stream_data * o_output_stream_data_new(struct o_output_stream *stream)
{
	struct o_output_stream_data * data = o_malloc(sizeof(struct o_output_stream_data));
	memset(data, 0, sizeof(struct o_output_stream_data));
	data->base.o_output_stream_write_bytes = o_output_stream_date_write_bytes;
	data->base.o_output_stream_flush = o_output_stream_data_flush;
	data->base.o_output_stream_free = o_output_stream_data_internal_free;
	data->wrapped = stream;
	return data;
}

