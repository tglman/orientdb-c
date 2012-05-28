#include "o_record_id.h"
#include "o_memory.h"
#include "o_string_buffer.h"

#define HASH_PRIME 31

struct o_record_id
{
	int cluster_id;
	long long record_id;
	int ref_count;
	char * string_id;
};

struct o_record_id * o_record_id_new_empty()
{
	return o_record_id_new(CLUSTER_ID_INVALID, CLUSTER_POS_INVALID);
}

struct o_record_id * o_record_id_new(int cluster_id, long long record_id)
{
	struct o_record_id * record = o_malloc(sizeof(struct o_record_id));
	record->cluster_id = cluster_id;
	record->record_id = record_id;
	record->ref_count = 1;
	record->string_id = 0;
	return record;
}

const int o_record_id_cluster_id(struct o_record_id * o_id)
{
	return o_id->cluster_id;
}

const long long o_record_id_record_id(struct o_record_id * o_id)
{
	return o_id->record_id;
}

int o_record_id_is_new(struct o_record_id * o_id)
{
	return o_id->record_id < 0;
}

void o_record_id_free(struct o_record_id * o_id)
{
	o_free(o_id);
}

void o_record_id_refer(struct o_record_id * o_id)
{
	o_id->ref_count++;
}

unsigned int o_record_id_hash(struct o_record_id * rid)
{
	return (o_record_id_cluster_id(rid) * HASH_PRIME + (o_record_id_record_id(rid) >> 32) * HASH_PRIME);
}

int o_record_id_compare(struct o_record_id * rid1, struct o_record_id * rid2)
{
	int dif = o_record_id_cluster_id(rid1) - o_record_id_cluster_id(rid2);
	if (dif != 0)
		return dif;
	return o_record_id_record_id(rid1) - o_record_id_record_id(rid2);
}

char * o_record_id_string(struct o_record_id * o_id)
{
	if (o_id->string_id == 0)
	{
		struct o_string_buffer * buff = o_string_buffer_new();
		o_string_buffer_append_long(buff, o_id->cluster_id);
		o_string_buffer_append_char(buff, ':');
		o_string_buffer_append_long(buff, o_id->record_id);
		o_id->string_id = o_string_buffer_str(buff);
		o_string_buffer_free(buff);
	}
	return o_id->string_id;
}

void o_record_id_release(struct o_record_id * o_id)
{
	o_id->ref_count--;
	if (o_id->ref_count == 0)
		o_record_id_free(o_id);
	if (o_id->string_id != 0)
		o_free(o_id->string_id);
}
