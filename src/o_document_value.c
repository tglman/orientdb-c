#include "o_document_value.h"
#include "o_document.h"
#include "o_document_internal.h"
#include "o_record.h"
#include "o_memory.h"
#include "o_exceptions.h"
#include <string.h>
#include "o_string_printer.h"
#include "o_string_buffer.h"

#define VALUE(obb,type) *((type *)obb->value)
#define VALUE_CHECK(OBB,TYPE,E_TYPE) E_TYPE==OBB->type?*((TYPE *)OBB->value):(TYPE)0

struct o_document_value * o_document_value_deserialize(struct o_input_stream * stream);

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
	int size = strlen(val) + 1;
	char * new_val = o_malloc(sizeof(char) * size);
	memcpy(new_val, val, size);
	VALUE(doc_val,char *) = new_val;
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

void o_document_value_serialize(struct o_document_value * o_value, struct o_string_printer *buff)
{
	switch (o_value->type)
	{
	case BOOL:
		if (VALUE(o_value,int))
			o_string_printer_print(buff, "true");
		else
			o_string_printer_print(buff, "false");
		break;
	case STRING:
		o_string_printer_print(buff, "\"");
		char * val = VALUE(o_value,char *);
		while (*val != 0)
		{
			if (*val == '"' || *val == '\'' || *val == '\\')
				o_string_printer_print_char(buff, '\\');
			o_string_printer_print_char(buff, *val++);
		}
		o_string_printer_print(buff, "\"");
		break;
	case BYTE:
		o_string_printer_print_long(buff, VALUE(o_value,char));
		break;
	case INT:
		o_string_printer_print_long(buff, VALUE(o_value,int));
		break;
	case LONG:
		o_string_printer_print_long(buff, VALUE(o_value,long));
		break;
	case SHORT:
		o_string_printer_print_long(buff, VALUE(o_value,short));
		break;
	case FLOAT:
		o_string_printer_print_double(buff, VALUE(o_value,float));
		break;
	case DOUBLE:
		o_string_printer_print_double(buff, VALUE(o_value,double));
		break;
	case EMBEDDED:
		o_string_printer_print(buff, "(");
		o_document_serialize_printer(VALUE(o_value,struct o_document *), buff);
		o_string_printer_print(buff, ")");
		break;
	case LINK:
	{
		struct o_record *rec = o_document_o_record(VALUE(o_value,struct o_document *));
		struct o_record_id * id = o_record_get_id(rec);
		o_string_printer_print(buff, "#");
		o_string_printer_print_long(buff, o_record_id_cluster_id(id));
		o_string_printer_print(buff, ":");
		o_string_printer_print_long(buff, o_record_id_record_id(id));
	}
		break;

	case ARRAY:
	{
		o_string_printer_print(buff, "[");
		int s = o_document_value_get_array_size(o_value);
		int i;
		struct o_document_value ** array = o_document_value_get_array(o_value);
		for (i = 0; i < s; i++)
		{
			o_document_value_serialize(array[i], buff);
			if (i < s)
				o_string_printer_print(buff, ",");
		}
		o_string_printer_print(buff, "]");
	}
		break;
	case DATE:
		break;

	}
}

struct o_document_value * o_document_value_embedded_deserialize(struct o_input_stream * stream)
{
	struct o_document *doc = o_document_new();
	o_document_deserialize(doc, stream);
	o_input_stream_read(stream);//remove , from the input stream.
	return o_document_value_embedded(doc);
}

struct o_document_value * o_document_value_string_deserialize(struct o_input_stream * stream)
{
	int readed;
	struct o_string_buffer * buff = o_string_buffer_new();
	while ((readed = o_input_stream_read(stream)) != '"')
	{
		if (readed == '\\')
			readed = o_input_stream_read(stream);
		o_string_buffer_append_char(buff, readed);
	}
	o_input_stream_read(stream);//remove , from the input stream.
	char * str = o_string_buffer_str(buff);
	struct o_document_value *value = o_document_value_string(str);
	o_free(str);
	o_string_buffer_free(buff);
	return value;
}

struct o_document_value * o_document_value_link_deserialize(struct o_input_stream * stream)
{
	int readed;
	int cid;
	long long rid;
	struct o_string_buffer * buff = o_string_buffer_new();
	while ((readed = o_input_stream_read(stream)) != ',')
	{
		if (readed == ':')
		{
			char * ca = o_string_buffer_str(buff);
			cid = atol(ca);
			o_free(ca);
			o_string_buffer_clear(buff);
			readed = o_input_stream_read(stream);
		}
		else if ((readed < '0' || readed > '9'))
		{
			o_string_buffer_free(buff);
			throw(o_exception_new("wrong link parsing", 0));
		}
		o_string_buffer_append_char(buff, readed);
	}
	char * ca = o_string_buffer_str(buff);
	rid = atol(ca);
	o_free(ca);
	o_string_buffer_free(buff);
	struct o_record_id * o_rid = o_record_id_new(cid, rid);
	return o_document_value_link(o_document_new_id(o_rid));
}

struct o_document_value * o_document_value_number_deserialize(struct o_input_stream * stream)
{
	int readed;
	struct o_string_buffer * buff = o_string_buffer_new();
	short doubleVal = 0;
	do
	{
		o_string_buffer_append_char(buff, readed);
		if (readed == '.')
			doubleVal = 1;
		else if ((readed < '0' || readed > '9'))
		{
			o_string_buffer_free(buff);
			throw(o_exception_new("wrong number parsing", 0));
		}
		readed = o_input_stream_read(stream);
	} while (readed != ',');
	char * ca = o_string_buffer_str(buff);
	struct o_document_value *value;
	if (doubleVal)
	{
		double val = atof(ca);
		value = o_document_value_long(val);
	}
	else
	{
		long val = atol(ca);
		value = o_document_value_long(val);
	}
	o_free(ca);
	o_string_buffer_free(buff);

	return value;
}
struct o_document_value_list
{
	struct o_document_value *val;
	struct o_document_value_list * next;
};

struct o_document_value * o_document_value_array_deserialize(struct o_input_stream * stream)
{
	struct o_document_value_list *value_list = 0;
	struct o_document_value_list *root = 0;
	int size = 0;
	struct o_document_value *value = 0;
	int readed;
	do
	{
		struct o_document_value *cur = o_document_value_deserialize(stream);
		if (value_list == 0)
			root = value_list = o_malloc(sizeof(struct o_document_value_list));
		else
			value_list = value_list->next = o_malloc(sizeof(struct o_document_value_list));
		value_list->val = cur;
		size++;
		readed = o_input_stream_read(stream);
	} while (readed != ']');
	o_input_stream_read(stream);//remove , from the input stream.

	struct o_document_value ** array = o_malloc(sizeof(struct o_document_value *) * size);
	int i = 0;
	value_list = root;
	while (i < size)
	{
		array[i++] = value_list->val;
		struct o_document_value_list *cur = value_list;
		value_list = value_list->next;
		o_free(cur);
	}
	value = o_document_value_array(array, size);
	o_free(array);
	return value;
}

struct o_document_value * o_document_value_map_deserialize(struct o_input_stream * stream)
{
	struct o_document_value *value = 0;
	int readed;
	do
	{
		readed = o_input_stream_read(stream);
	} while (readed != '}');
	o_input_stream_read(stream);//remove , from the input stream.
	return value;
}
struct o_document_value * o_document_value_bool_deserialize(struct o_input_stream * stream, int istrue)
{
	o_input_stream_read(stream);
	o_input_stream_read(stream);
	o_input_stream_read(stream);
	o_input_stream_read(stream);
	if (!istrue)
	{
		o_input_stream_read(stream);
	}
	return o_document_value_bool(istrue);
}

struct o_document_value * o_document_value_deserialize(struct o_input_stream * stream)
{
	int readed = o_input_stream_read(stream);
	switch (readed)
	{
	case '*':
		return o_document_value_embedded_deserialize(stream);
	case ',':
		return 0;
	case '"':
		return o_document_value_string_deserialize(stream);
	case '#':
		return o_document_value_link_deserialize(stream);
	case '[':
		return o_document_value_array_deserialize(stream);
	case '{':
		return o_document_value_map_deserialize(stream);
	case '(':
		return o_document_value_embedded_deserialize(stream);
	case 't':
	case 'f':
		return o_document_value_bool_deserialize(stream, readed == 't');
	default:
		return o_document_value_number_deserialize(stream);
	}
}
void o_document_value_free(struct o_document_value * to_free)
{
	/*if (to_free->type == LINK || to_free->type == EMBEDDED)
	 o_document_free(VALUE(to_free,struct o_document *));

	 else*/if (to_free->type == STRING)
		o_free(VALUE(to_free,char *));
	o_free(to_free->value);
	o_free(to_free);
}
