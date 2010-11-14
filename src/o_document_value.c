#include "o_document_value.h"
#include "o_document.h"
#include "o_record.h"
#include "o_memory.h"
#define VALUE(obb,type) *((type *)obb->value)
#define VALUE_CHECK(OBB,TYPE,E_TYPE) E_TYPE==OBB->type?*((TYPE *)OBB->value):(TYPE)0

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
	//TODO: strdup
	VALUE(doc_val,char *) = val;
	return doc_val;
}

struct o_document_value * o_document_value_embedded(struct o_document * doc)
{
	struct o_document_value * doc_val = o_document_value_new(EMBEDDED, sizeof(struct o_document *));
	VALUE(doc_val,struct o_document *) = doc;
	return doc_val;
}

struct o_document_value * o_document_value_link(struct o_document * doc)
{
	struct o_document_value * doc_val = o_document_value_new(LINK, sizeof(struct o_document *));
	VALUE(doc_val,struct o_document *) = doc;
	return doc_val;
}

struct o_document_value * o_document_value_array(struct o_document_value * array[], int size)
{
	struct o_document_value * doc_val = o_document_value_new(ARRAY, sizeof(struct o_document_value_array));
	struct o_document_value_array * val = doc_val->value;
	val->array = array;
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

struct o_document * o_document_value_get_link(struct o_document_value * o_value)
{
	return VALUE_CHECK(o_value,struct o_document *,LINK);
}

struct o_document_value ** o_document_value_get_array(struct o_document_value * o_value)
{
	if (o_value->type != ARRAY)
		return 0;
	else
		return ((struct o_document_value_array *) o_value->value)->array;
}
int o_document_value_get_array_size(struct o_document_value * o_value)
{
	if (o_value->type != ARRAY)
		return 0;
	else
		return ((struct o_document_value_array *) o_value->value)->size;
}

void o_document_value_serialize(struct o_document_value * o_value, struct o_string_buffer *buff)
{
	switch (o_value->type)
	{
	case BOOL:
		if (VALUE(o_value,int))
			o_string_buffer_append(buff, "true");
		else
			o_string_buffer_append(buff, "false");
		break;
	case STRING:
		o_string_buffer_append(buff, "\"");
		o_string_buffer_append(buff, VALUE(o_value,char *));
		o_string_buffer_append(buff, "\"");
		break;
	case BYTE:
		o_string_buffer_append_long(buff, VALUE(o_value,char));
		break;
	case INT:
		o_string_buffer_append_long(buff, VALUE(o_value,int));
		break;
	case LONG:
		o_string_buffer_append_long(buff, VALUE(o_value,long));
		break;
	case SHORT:
		o_string_buffer_append_long(buff, VALUE(o_value,short));
		break;
	case FLOAT:
		o_string_buffer_append_double(buff, VALUE(o_value,float));
		break;
	case DOUBLE:
		o_string_buffer_append_double(buff, VALUE(o_value,double));
		break;
	case EMBEDDED:
		o_string_buffer_append(buff, "*");
		o_document_serialize(VALUE(o_value,struct o_document *), buff);
		o_string_buffer_append(buff, "*");
		break;
	case LINK:
	{
		struct o_record *rec = o_document_o_record(VALUE(o_value,struct o_document *));
		struct o_record_id * id = o_record_get_id(rec);
		o_string_buffer_append(buff, "#");
		o_string_buffer_append_long(buff, o_record_id_cluster_id(id));
		o_string_buffer_append(buff, ":");
		o_string_buffer_append_long(buff, o_record_id_record_id(id));
	}
		break;

	case ARRAY:
	{
		o_string_buffer_append(buff, "[");
		int s = o_document_value_get_array_size(o_value);
		int i;
		struct o_document_value ** array = o_document_value_get_array(o_value);
		for (i = 0; i < s; i++)
		{
			o_document_value_serialize(array[i], buff);
			if (i < s)
				o_string_buffer_append(buff, ",");
		}
		o_string_buffer_append(buff, "]");
	}
		break;
	case DATE:
		break;

	}
}

void o_document_value_free(struct o_document_value * to_free)
{
	o_free(to_free->value);
	o_free(to_free);
}
