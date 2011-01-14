#include "o_record_raw.h"
#include "o_record_internal.h"
#include "o_memory.h"
#include <string.h>

struct o_record_raw
{
	struct o_record record;
	char * content;
	int content_len;
};

void o_record_raw_serialize(struct o_record * record, struct o_output_stream * stream)
{
	struct o_record_raw * raw = (struct o_record_raw *) record;
	o_output_stream_write_bytes(stream, raw->content, raw->content_len);
}

void o_record_raw_reset(struct o_record * record, char * content, int content_len)
{
	struct o_record_raw * raw = (struct o_record_raw *) record;
	if (raw->content != 0)
		o_free(raw->content);
	raw->content = o_memdup(content, content_len);
	raw->content_len = content_len;
}

void o_record_raw_deserialize(struct o_record * record, struct o_input_stream * stream)
{
	struct o_record_raw * raw = (struct o_record_raw *) record;
	if (raw->content != 0)
	{
		o_free(raw->content);
		raw->content = 0;
	}
	int readed = 0;
	do
	{
		if (raw->content == 0)
		{
			raw->content = o_malloc(521);
			raw->content_len = 0;
		}
		else
			raw->content = o_realloc(raw->content, raw->content_len + 512);

		readed = o_input_stream_read_bytes(stream, raw->content + raw->content_len, 512);
		raw->content_len += readed;
	} while (readed == 512);

}

void o_record_raw_free(struct o_record * record)
{
	o_record_free_internal(record);
	struct o_record_raw * raw = (struct o_record_raw *) record;
	if (raw->content != 0)
		o_free(raw->content);
}

char * o_record_raw_content(struct o_record * record, int *content_len)
{
	struct o_record_raw * raw = (struct o_record_raw *) record;
	*content_len = raw->content_len;
	return raw->content;
}

void o_record_raw_before_save(struct o_record * record)
{
}

void o_record_raw_after_save(struct o_record * record)
{
}

char * o_record_raw_cluster_name(struct o_record * record)
{
	return 0;
}

struct o_record * o_record_raw_new_id(struct o_record_id * rid)
{
	struct o_record_raw * rr = o_malloc(sizeof(struct o_record_raw));
	memset(rr, 0, sizeof(struct o_record_raw));
	o_record_new_internal_id(&rr->record, RAW_RECORD_TYPE, rid);
	rr->record.o_record_deserialize = o_record_raw_deserialize;
	rr->record.o_record_serialize = o_record_raw_serialize;
	rr->record.o_record_before_save = o_record_raw_before_save;
	rr->record.o_record_after_save = o_record_raw_after_save;
	rr->record.o_record_cluster_name = o_record_raw_cluster_name;
	rr->record.o_record_free = o_record_raw_free;
	return &rr->record;
}

struct o_record * o_record_raw_new()
{
	return o_record_raw_new_id(o_record_id_new_empty());
}

struct o_record * o_record_raw_new_content(char * content, int content_len)
{
	struct o_record * rec = o_record_raw_new();
	o_record_raw_reset(rec, content, content_len);
	return rec;
}
