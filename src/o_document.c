#include "o_document.h"
#include "o_map.h"
#include "o_record_internal.h"
#include "o_memory.h"
#include "o_document_value.h"
#include "o_string_buffer.h"
#include <stdio.h>
#include <string.h>

struct o_document
{
	struct o_record record;
	char * class_name;
	struct o_map *fields;
	struct o_map *fields_old_values;
};

void o_document_free_maps_values(struct o_document * doc);

void o_document_record_serialize(struct o_record * rec, struct o_output_stream * buff)
{
	struct o_string_buffer *b = o_string_buffer_new();
	o_document_serialize((struct o_document *) rec, b);
	char * bytes = o_string_buffer_str(b);
	o_output_stream_write_bytes(buff, bytes, strlen(bytes));
	o_free(bytes);
	o_string_buffer_free(b);
}

struct o_document * o_document_new()
{
	struct o_document * new_doc = o_malloc(sizeof(struct o_document));
	o_record_new_internal(o_document_o_record(new_doc), 'd');
	new_doc->record.o_record_serialize = o_document_record_serialize;
	new_doc->fields = o_map_new();
	new_doc->fields_old_values = 0;
	return new_doc;
}
struct o_document * o_document_new_id(struct o_record_id * rid)
{
	struct o_document * new_doc = o_malloc(sizeof(struct o_document));
	o_record_new_internal_id(o_document_o_record(new_doc), 'd', rid);
	new_doc->record.o_record_serialize = o_document_record_serialize;
	new_doc->fields = o_map_new();
	new_doc->fields_old_values = 0;
	return new_doc;
}

struct o_record * o_document_o_record(struct o_document * doc)
{
	return &doc->record;
}

struct o_document_value * o_document_field_get(struct o_document * doc, char * field_name)
{
	void * val = o_map_get(doc->fields, field_name);
	return (struct o_document_value *) val;
}

void o_document_field_set(struct o_document * doc, char * field_name, struct o_document_value* value)
{
	struct o_document_value * old = o_map_put(doc->fields, field_name, value);
	if (old != 0)
	{
		if (doc->fields_old_values == 0)
			doc->fields_old_values = o_map_new();
		o_map_put(doc->fields_old_values, field_name, old);
	}
}

char ** o_document_field_names(struct o_document * doc, int *names_count)
{
	return o_map_keys(doc->fields, names_count);
}

struct o_document_value ** o_document_field_values(struct o_document * doc, int *values_count)
{
	return (struct o_document_value **) o_map_values(doc->fields, values_count);
}

int o_document_contains_field(struct o_document *doc, char * field_name)
{
	return o_map_get(doc->fields, field_name) != 0;
}

void o_document_remove_field(struct o_document *doc, char * field_name)
{
	o_map_remove(doc->fields, field_name);
}

struct o_document * o_document_copy(struct o_document * doc)
{
	struct o_document * new_doc = o_document_new();
	int i;
	int size;
	char ** keys = o_map_keys(doc->fields, &size);
	for (i = 0; i < size; ++i)
		o_map_put(new_doc->fields, keys[i], o_map_get(new_doc->fields, keys[i]));
	return new_doc;
}

void o_document_serialize(struct o_document * doc, struct o_string_buffer * buff)
{
	int names_count;
	char ** names = o_document_field_names(doc, &names_count);
	int i;
	for (i = 0; i < names_count; i++)
	{
		o_string_buffer_append(buff, names[i]);
		o_string_buffer_append(buff, ":");
		o_document_value_serialize(o_document_field_get(doc, names[i]), buff);
		if (i < names_count - 1)
			o_string_buffer_append(buff, ",");
	}
}

char * o_document_get_class_name(struct o_document * doc)
{
	return doc->class_name;
}

void o_document_deserialize_internal(struct o_document * doc, struct o_input_stream * stream, int embeddd)
{
	struct o_string_buffer * buff = o_string_buffer_new();
	int isfirst = 1;
	int readed;
	do
	{
		readed = o_input_stream_read(stream);
		if (isfirst && readed == '@')
		{
			char * str = o_string_buffer_str(buff);
			doc->class_name = str;
			o_string_buffer_clear(buff);
		}
		else if (readed == ':')
		{
			isfirst = 0;
			char * str = o_string_buffer_str(buff);
			struct o_document_value * val = o_document_value_deserialize(stream);
			o_document_field_set(doc, str, val);
			o_free(str);
			o_string_buffer_clear(buff);
		}
		else if (readed != -1)
		{
			o_string_buffer_append_char(buff, readed);
		}
	} while (readed != -1);
	o_string_buffer_free(buff);
}

void o_document_deserialize(struct o_document * doc, struct o_input_stream * stream)
{
	o_document_deserialize_internal(doc, stream, 0);
}

void o_document_free_maps_values(struct o_document * doc)
{
	int values_count;
	struct o_document_value **values = o_document_field_values(doc, &values_count);
	int i;
	for (i = 0; i < values_count; i++)
		o_document_value_free(values[i]);

	if (doc->fields_old_values != 0)
	{
		values = (struct o_document_value **) o_map_values(doc->fields_old_values, &values_count);
		for (i = 0; i < values_count; i++)
			o_document_value_free(values[i]);
	}
}

void o_document_free(struct o_document * doc)
{
	o_document_free_maps_values(doc);
	o_map_free(doc->fields);
	if (doc->fields_old_values != 0)
		o_map_free(doc->fields_old_values);
	o_record_free_internal(&doc->record);
	o_free(doc);
}

