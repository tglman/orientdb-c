#include "o_record_internal.h"
#include "o_row_buffer_record.h"
#include "o_memory.h"

struct o_record * o_record_new()
{
	struct o_record * record = o_malloc(sizeof(struct o_record));
	o_record_new_internal(record, 'b');
	return record;
}

void o_record_new_internal(struct o_record * record, char type)
{
	record->record_id = o_record_id_new_empty();
	record->type = type;
	record->version = 0;
}

struct o_record_id * o_record_get_id(struct o_record * record)
{
	return record->record_id;
}

void o_record_free_internal(struct o_record * record)
{
	o_record_id_free(record->record_id);
}

int o_record_version(struct o_record * record)
{
	return record->version;
}

char o_record_type(struct o_record * record)
{
	return record->type;
}

struct o_raw_buffer * o_record_content(struct o_record * record)
{
	return o_raw_buffer_record(record);
}

void o_record_serialize(struct o_record * record, struct o_output_stream * output)
{
	record->o_record_serialize(record, output);
}

void o_record_free(struct o_record * record)
{
	o_record_free_internal(record);
	o_free(record);
}
