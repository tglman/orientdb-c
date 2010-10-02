#include "o_record_id.h"
#include <malloc.h>

#define CLUSTER_ID_INVALID	-1
#define CLUSTER_POS_INVALID	 -1

struct o_record_id
{
	int cluster_id;
	long long record_id;
};

struct o_record_id * o_record_id_new_empty()
{
	return o_record_id_new(CLUSTER_ID_INVALID, CLUSTER_POS_INVALID);
}

struct o_record_id * o_record_id_new(int cluster_id, long long record_id)
{
	struct o_record_id * record = malloc(sizeof(struct o_record_id));
	record->cluster_id = cluster_id;
	record->record_id = record_id;
	return record;
}

int o_record_id_cluster_id(struct o_record_id * o_id)
{
	return o_id->cluster_id;
}

long long o_record_id_record_id(struct o_record_id * o_id)
{
	return o_id->cluster_id;
}

void o_record_id_free(struct o_record_id * o_id)
{
	free(o_id);
}
