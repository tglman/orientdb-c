#include "o_record_internal.h"
#include "o_raw_buffer_record.h"
#include "o_memory.h"

void o_record_new_internal(struct o_record * record, char type)
{
	record->record_id = o_record_id_new_empty();
	record->type = type;
	record->version = 0;
	record->ref_count = 1;
}

void o_record_new_internal_id(struct o_record * record, char type, struct o_record_id *rid)
{
	record->record_id = rid;
	record->type = type;
	record->version = 0;
	record->ref_count = 1;
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

void o_record_deserialize(struct o_record * record, struct o_input_stream * input)
{
	record->o_record_deserialize(record, input);
}

void o_record_refer(struct o_record * record)
{
	record->ref_count++;
}

void o_record_release(struct o_record * record)
{
	record->ref_count--;
	if (record->ref_count == 0)
		record->o_record_free(record);
}

