#include "o_record_internal.h"
#include "o_raw_buffer_record.h"
#include "o_database.h"
#include "o_memory.h"

void o_record_new_internal_id(struct o_record * record, char type, struct o_record_id *rid)
{
	record->record_id = rid;
	record->type = type;
	record->version = 0;
	record->ref_count = 1;
	record->owner = o_database_context_database();
}

void o_record_new_internal(struct o_record * record, char type)
{
	o_record_new_internal_id(record, type, o_record_id_new_empty());
}

struct o_record_id * o_record_get_id(struct o_record * record)
{
	return record->record_id;
}

void o_record_reset_id(struct o_record * record, struct o_record_id *new_id)
{
	if (record->record_id != 0)
		o_record_id_release(record->record_id);
	record->record_id = new_id;
}

void o_record_free_internal(struct o_record * record)
{
	o_record_id_release(record->record_id);
}

int o_record_version(struct o_record * record)
{
	return record->version;
}

char o_record_type(struct o_record * record)
{
	return record->type;
}

void o_record_reset_version(struct o_record * record, int new_version)
{
	record->version = new_version;
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

void o_record_before_save(struct o_record * record)
{
	record->o_record_before_save(record);
}

void o_record_after_save(struct o_record * record)
{
	record->o_record_after_save(record);
}

void o_record_refer(struct o_record * record)
{
	record->ref_count++;
}

struct o_database * o_record_owner(struct o_record * record)
{
	return record->owner;
}

void o_record_release(struct o_record * record)
{
	record->ref_count--;
	if (record->ref_count == 0)
		record->o_record_free(record);
}

