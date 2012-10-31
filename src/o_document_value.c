#include "o_document_value.h"
#include "o_database_internal.h"
#include "o_document_internal.h"
#include "o_record.h"
#include "o_memory.h"
#include "o_exceptions.h"
#include <string.h>
#include "o_string_buffer.h"
#include <stdio.h>

#define VALUE(obb,type) *((type *)obb->value)
#define VALUE_CHECK(OBB,TYPE,E_TYPE) E_TYPE==OBB->type?*((TYPE *)OBB->value):(TYPE)0

struct o_document_value_link
{
	struct o_database * db;
	struct o_record_id * rid;
	struct o_record *record;
};

struct o_document_value
{
	enum o_document_value_type type;
	void *value;
};
struct o_document_value_array
{
	struct o_document_value **array;
	int size;
};

struct o_document_value * o_document_value_new(enum o_document_value_type type, int value_size)
{
	struct o_document_value *val = o_malloc(sizeof(struct o_document_value));
	val->type = type;
	val->value = o_malloc(value_size);
	return val;
}

struct o_document_value * o_document_value_byte(char val)
{
	struct o_document_value * doc_val = o_document_value_new(BYTE, sizeof(char));
	VALUE(doc_val,char) = val;
	return doc_val;
}

struct o_document_value * o_document_value_int(int val)
{
	struct o_document_value * doc_val = o_document_value_new(INT, sizeof(int));
	VALUE(doc_val,int) = val;
	return doc_val;
}

struct o_document_value * o_document_value_short(short val)
{
	struct o_document_value * doc_val = o_document_value_new(SHORT, sizeof(int));
	VALUE(doc_val,short) = val;
	return doc_val;
}

struct o_document_value * o_document_value_long(long val)
{
	struct o_document_value * doc_val = o_document_value_new(LONG, sizeof(long));
	VALUE(doc_val,long) = val;
	return doc_val;
}

struct o_document_value * o_document_value_bool(int val)
{
	struct o_document_value * doc_val = o_document_value_new(BOOL, sizeof(int));
	VALUE(doc_val,int) = val;
	return doc_val;
}

struct o_document_value * o_document_value_float(float val)
{
	struct o_document_value * doc_val = o_document_value_new(FLOAT, sizeof(float));
	VALUE(doc_val,float) = val;
	return doc_val;
}

struct o_document_value * o_document_value_double(double val)
{
	struct o_document_value * doc_val = o_document_value_new(DOUBLE, sizeof(double));
	*((double *) doc_val->value) = val;
	return doc_val;
}

struct o_document_value * o_document_value_date(int val)
{
	struct o_document_value * doc_val = o_document_value_new(DATE, sizeof(int));
	VALUE(doc_val,int) = val;
	return doc_val;
}

struct o_document_value * o_document_value_string(char * val)
{
	struct o_document_value * doc_val = o_document_value_new(STRING, sizeof(char *));
	VALUE(doc_val,char *) = o_memdup(val, strlen(val) + 1);
	;
	return doc_val;
}

struct o_document_value * o_document_value_embedded(struct o_document * doc)
{
	struct o_document_value * doc_val = o_document_value_new(EMBEDDED, sizeof(struct o_document *));
	o_record_refer(doc);
	VALUE(doc_val,struct o_document *) = doc;
	return doc_val;
}

struct o_document_value * o_document_value_link_ref(struct o_record_id *id)
{
	struct o_document_value_link *link = o_malloc(sizeof(struct o_document_value_link));
	link->rid = id;
	o_record_id_refer(id);
	link->record = 0;
	link->db = o_database_context_database();
	o_database_add_referrer(link->db, &link->db);
	struct o_document_value * doc_val = o_document_value_new(LINK, sizeof(struct o_document_value_link *));
	VALUE(doc_val,struct o_document_value_link *) = link;
	return doc_val;

}

struct o_document_value * o_document_value_link(struct o_record * rec)
{
	struct o_document_value_link *link = o_malloc(sizeof(struct o_document_value_link));
	memset(link, 0, sizeof(struct o_document_value_link));
	link->record = rec;
	o_record_refer(rec);
	struct o_document_value * doc_val = o_document_value_new(LINK, sizeof(struct o_document_value_link *));
	VALUE(doc_val,struct o_document_value_link *) = link;
	return doc_val;
}

struct o_document_value * o_document_value_array(struct o_document_value * array[], int size)
{
	struct o_document_value * doc_val = o_document_value_new(ARRAY, sizeof(struct o_document_value_array));
	struct o_document_value_array * val = doc_val->value;
	val->array = o_malloc(sizeof(struct o_document_value *) * size);
	memcpy(val->array, array, sizeof(struct o_document_value *) * size);
	val->size = size;
	return doc_val;
}

enum o_document_value_type o_document_value_type(struct o_document_value * value)
{
	return value->type;
}

char o_document_value_get_byte(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,char,BYTE);
}

short o_document_value_get_short(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,short,SHORT);
}

int o_document_value_get_int(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,int,INT);
}

int o_document_value_get_bool(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,int,BOOL);
}

long o_document_value_get_long(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,long,LONG);
}

float o_document_value_get_float(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,float,FLOAT);
}

double o_document_value_get_double(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,double,DOUBLE);
}

int o_document_value_get_date(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,int,DATE);
}

char * o_document_value_get_string(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,char *,STRING);
}

struct o_document * o_document_value_get_embedded(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,struct o_document *,EMBEDDED);
}

struct o_record * o_document_value_get_link(struct o_document_value * o_value)
{
	struct o_document_value_link * link = VALUE_CHECK(o_value,struct o_document_value_link *,LINK);
	if (link == 0)
		return 0;
	if (link->record == 0)
	{
		if (link->db == 0 || link->rid == 0)
			return 0;
		link->record = o_database_load(link->db, link->rid);
		o_database_remove_referrer(link->db, &link->db);
	}
	return link->record;
}

struct o_record_id * o_document_value_get_link_ref(struct o_document_value * o_value)
{
	struct o_document_value_link * link = VALUE_CHECK(o_value,struct o_document_value_link *,LINK);
	if (link == 0)
		return 0;
	if (link->record == 0)
		return link->rid;
	return o_record_get_id(link->record);

}

struct o_document_value ** o_document_value_get_array(struct o_document_value * o_value, int *array_size)
{
	if (o_value->type != ARRAY)
		return 0;
	*array_size = ((struct o_document_value_array *) o_value->value)->size;
	return ((struct o_document_value_array *) o_value->value)->array;
}

int o_document_value_get_array_size(struct o_document_value * o_value)
{
	if (o_value->type != ARRAY)
		return 0;
	else
		return ((struct o_document_value_array *) o_value->value)->size;
}

void o_document_value_free(struct o_document_value * to_free)
{
	if (to_free == 0)
		return;
	if (to_free->type == LINK)
	{
		struct o_document_value_link * val = VALUE(to_free,struct o_document_value_link *);
		if (val->db != 0)
			o_database_remove_referrer(val->db, &val->db);
		if (val->rid != 0)
			o_record_id_release(val->rid);
		if (val->record != 0)
			o_record_release(val->record);
		o_free(val);
	}
	else if (to_free->type == EMBEDDED)
	{
		struct o_document * doc = VALUE_CHECK(to_free,struct o_document *,EMBEDDED);
		if (doc != 0)
			o_document_release(doc);
	}
	else if (to_free->type == ARRAY)
	{
		int i = ((struct o_document_value_array *) to_free->value)->size;
		while (i > 0)
		{
			if (((struct o_document_value_array *) to_free->value)->array[--i] != 0)
				o_document_value_free(((struct o_document_value_array *) to_free->value)->array[i]);
		}
		o_free(((struct o_document_value_array *) to_free->value)->array);
	}
	else if (to_free->type == STRING)
		o_free(VALUE(to_free,char *) );
	o_free(to_free->value);
	o_free(to_free);
}
