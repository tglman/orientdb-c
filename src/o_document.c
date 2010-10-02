#include "o_document.h"
#include "o_map.h"
#include "o_record_internal.h"
#include "o_memory.h"

struct o_document
{
	struct o_record record;
	struct o_map *fields;
	struct o_map *fields_old_values;
};

struct o_document * o_document_new()
{
	struct o_document * new_doc = o_malloc(sizeof(struct o_document));
	o_record_new_internal(o_document_o_record(new_doc));
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

void o_document_free(struct o_document * doc)
{
	o_map_free(doc->fields);
	if (doc->fields_old_values != 0)
		o_map_free(doc->fields_old_values);
	o_record_free_internal(&doc->record);
	o_free(doc);
}

