#include "o_row_buffer_internal.h"
#include "o_row_buffer_record.h"
#include "o_output_stream_byte.h"
#include "o_memory.h"

struct o_raw_buffer_record
{
	struct o_raw_buffer buffer;
	struct o_record *record;
	char * content;
	int size;
};

void o_raw_buffer_record_retrieve_content(struct o_raw_buffer_record * row_buff)
{
	struct o_output_stream * o = o_output_stream_byte_buffer();
	o_record_serialize(row_buff->record, o);
	row_buff->content = o_output_stream_byte_content(o, &row_buff->size);
	o_output_stream_free(o);
}

int o_raw_buffer_record_content_size(struct o_raw_buffer * buff)
{
	struct o_raw_buffer_record * row_buff = (struct o_raw_buffer_record *) buff;
	if (row_buff->content == 0)
		o_raw_buffer_record_retrieve_content(row_buff);
	return row_buff->size;
}
char * o_raw_buffer_record_content(struct o_raw_buffer * buff, int * content_size)
{
	struct o_raw_buffer_record * row_buff = (struct o_raw_buffer_record *) buff;
	if (row_buff->content == 0)
		o_raw_buffer_record_retrieve_content(row_buff);
	*content_size = row_buff->size;
	return row_buff->content;
}

int o_raw_buffer_record_version(struct o_raw_buffer * buff)
{
	struct o_raw_buffer_record * row_buff = (struct o_raw_buffer_record *) buff;
	return o_record_version(row_buff->record);
}

char o_raw_buffer_record_type(struct o_raw_buffer * buff)
{
	struct o_raw_buffer_record * row_buff = (struct o_raw_buffer_record *) buff;
	return o_record_type(row_buff->record);
}

struct o_raw_buffer * o_raw_buffer_record(struct o_record * record)
{
	struct o_raw_buffer_record * row_buff = o_malloc(sizeof(struct o_raw_buffer_record));
	row_buff->record = record;
	row_buff->content = 0;
	row_buff->size = 0;
	row_buff->buffer.o_raw_buffer_content_size = o_raw_buffer_record_content_size;
	row_buff->buffer.o_raw_buffer_content = o_raw_buffer_record_content;
	row_buff->buffer.o_raw_buffer_version = o_raw_buffer_record_version;
	row_buff->buffer.o_raw_buffer_type = o_raw_buffer_record_type;
	return (struct o_raw_buffer *) row_buff;
}
