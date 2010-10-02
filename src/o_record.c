#include "o_record_internal.h"
#include "o_memory.h"

struct o_record * o_record_new()
{
	struct o_record * record = o_malloc(sizeof(struct o_record));
	o_record_new_internal(record);
	return record;
}

void o_record_new_internal(struct o_record * record)
{
	record->record_id = o_record_id_new_empty();
	record->version = 0;
}

void o_record_free_internal(struct o_record * record)
{
	o_record_id_free(record->record_id);
}

void o_record_free(struct o_record * record)
{
	o_record_free_internal(record);
	o_free(record);
}
