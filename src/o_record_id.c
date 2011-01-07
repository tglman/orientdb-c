#include "o_record_id.h"
#include "o_memory.h"

#define CLUSTER_ID_INVALID	-1
#define CLUSTER_POS_INVALID	 -1

struct o_record_id
{
	int cluster_id;
	long long record_id;
	int ref_count;
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
	return record;
}

int o_record_id_cluster_id(struct o_record_id * o_id)
{
	return o_id->cluster_id;
}

long long o_record_id_record_id(struct o_record_id * o_id)
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

void o_record_id_release(struct o_record_id * o_id)
{
	o_id->ref_count--;
	if (o_id->ref_count == 0)
		o_record_id_free(o_id);
}
