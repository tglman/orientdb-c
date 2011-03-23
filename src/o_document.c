#include "o_document.h"
#include "o_map_string.h"
#include "o_record_internal.h"
#include "o_memory.h"
#include "o_document_value.h"
#include "o_string_buffer.h"
#include "o_database.h"
#include "o_database_document_internal.h"
#include "o_document_formatter.h"
#include <stdio.h>
#include <string.h>

struct o_document
{
	struct o_record record;
	char * class_name;
	struct o_class * class;
	struct o_map_string *fields;
	struct o_map_string *fields_old_values;
};

void o_document_free(struct o_document * doc);

void o_document_record_serialize(struct o_record * rec, struct o_output_stream* out)
{
	o_document_serialize((struct o_document *) rec, out);
}

void o_document_record_deserialize(struct o_record * rec, struct o_input_stream * buff)
{
	o_document_deserialize((struct o_document *) rec, buff);
}

void o_document_record_free(struct o_record * rec)
{
	o_document_free((struct o_document *) rec);
}

void o_document_before_save(struct o_record * rec)
{
	struct o_document * doc = (struct o_document *) rec;
	int values_count;
	struct o_document_value **values = o_document_field_values(doc, &values_count);
	int i;
	for (i = 0; i < values_count; i++)
	{
		if (o_document_value_type(values[i]) == LINK)
		{
			//TODO:Verfy correctly database use.
			struct o_record *link_rec = o_document_value_get_link(values[i]);
			o_database_save(o_database_context_database(), link_rec, 0);
		}
	}
}

void o_document_after_save(struct o_record * rec)
{
}

char *o_document_cluster_name(struct o_record * record)
{
	struct o_document * doc = (struct o_document *) record;
	return doc->class_name;
}
struct o_document * o_document_new_id(struct o_record_id * rid)
{
	struct o_document * new_doc = o_malloc(sizeof(struct o_document));
	o_record_new_internal_id(o_document_o_record(new_doc), DOCUMENT_RECORD_TYPE, rid);
	new_doc->record.o_record_serialize = o_document_record_serialize;
	new_doc->record.o_record_deserialize = o_document_record_deserialize;
	new_doc->record.o_record_before_save = o_document_before_save;
	new_doc->record.o_record_after_save = o_document_after_save;
	new_doc->record.o_record_cluster_name = o_document_cluster_name;
	new_doc->record.o_record_free = o_document_record_free;
	new_doc->fields = o_map_string_new();
	new_doc->fields_old_values = 0;
	return new_doc;
}

struct o_document * o_document_new()
{
	return o_document_new_id(o_record_id_new_empty());
}

struct o_record * o_document_o_record(struct o_document * doc)
{
	return &doc->record;
}

struct o_document_value * o_document_field_get(struct o_document * doc, char * field_name)
{
	void * val = o_map_string_get(doc->fields, field_name);
	return (struct o_document_value *) val;
}

void o_document_field_set(struct o_document * doc, char * field_name, struct o_document_value* value)
{
	struct o_document_value * old = o_map_string_put(doc->fields, field_name, value);
	if (old != 0)
	{
		if (doc->fields_old_values == 0)
			doc->fields_old_values = o_map_string_new();
		o_map_string_put(doc->fields_old_values, field_name, old);
	}
}

char ** o_document_field_names(struct o_document * doc, int *names_count)
{
	return o_map_string_keys(doc->fields, names_count);
}

struct o_document_value ** o_document_field_values(struct o_document * doc, int *values_count)
{
	return (struct o_document_value **) o_map_string_values(doc->fields, values_count);
}

int o_document_contains_field(struct o_document *doc, char * field_name)
{
	return o_map_string_get(doc->fields, field_name) != 0;
}

void o_document_remove_field(struct o_document *doc, char * field_name)
{
	o_map_string_remove(doc->fields, field_name);
}

struct o_document * o_document_copy(struct o_document * doc)
{
	struct o_document * new_doc = o_document_new();
	int i;
	int size;
	char ** keys = o_map_string_keys(doc->fields, &size);
	for (i = 0; i < size; ++i)
		o_map_string_put(new_doc->fields, keys[i], o_map_string_get(new_doc->fields, keys[i]));
	return new_doc;
}

void o_document_serialize(struct o_document * doc, struct o_output_stream * output)
{
	struct o_database_document * db = (struct o_database_document *) o_database_context_database();
	struct o_document_formatter * fm = 0;
	if (db != 0)
		fm = o_database_document_get_formatter(db);
	else
		fm = o_document_formatter_factory_default();
	o_document_formatter_serialize(fm, doc, output);
}

char * o_document_get_class_name(struct o_document * doc)
{
	if (doc->class_name == 0 && doc->class != 0)
		return o_class_get_name(doc->class);
	return doc->class_name;
}

void o_document_init_by_class_name(struct o_document * doc)
{
	if (doc->class_name != 0)
	{
		struct o_database * db = o_database_context_database();
		if (db != 0 && o_database_get_type(db) == DOCUMENT_DB_TYPE)
		{
			struct o_database_document * docDb = (struct o_database_document *) o_database_context_database();
			struct o_metadata * meta = o_database_document_metadata(docDb);
			if (meta != 0)
			{
				struct o_schema *schema = o_metadata_get_schema(meta);
				if (schema != 0)
					doc->class = o_schema_get_class(schema, doc->class_name);
			}
		}

	}
}

struct o_class * o_document_get_class(struct o_document * doc)
{
	if (doc->class == 0)
		o_document_init_by_class_name(doc);
	return doc->class;
}

void o_document_set_class(struct o_document * doc, struct o_class * cl)
{
	doc->class = cl;
}

void o_document_set_class_by_name(struct o_document * doc, char * class_name)
{
	doc->class_name = o_memdup(class_name, strlen(class_name));
	o_document_init_by_class_name(doc);
}

void o_document_deserialize(struct o_document * doc, struct o_input_stream * stream)
{
	struct o_database * db = o_database_context_database();
	struct o_document_formatter * fm = 0;
	if (db != 0 && o_database_get_type(db) == DOCUMENT_DB_TYPE)
		fm = o_database_document_get_formatter((struct o_database_document *) db);
	else
		fm = o_document_formatter_factory_default();
	o_document_formatter_deserialize(fm, doc, stream);
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
		values = (struct o_document_value **) o_map_string_values(doc->fields_old_values, &values_count);
		for (i = 0; i < values_count; i++)
			o_document_value_free(values[i]);
	}
}

void o_document_free(struct o_document * doc)
{
	o_document_free_maps_values(doc);
	o_map_string_free(doc->fields);
	if (doc->fields_old_values != 0)
		o_map_string_free(doc->fields_old_values);
	o_record_free_internal(&doc->record);
	o_free(doc);
}

void o_document_release(struct o_document * doc)
{
	o_record_release(&doc->record);
}

void o_document_refer(struct o_document * doc)
{
	o_record_refer(&doc->record);
}

