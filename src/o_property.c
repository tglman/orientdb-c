#include "o_property.h"
#include "o_property_internal.h"
#include "o_memory.h"
#include "o_document.h"
#include <string.h>

struct o_property
{
	char * name;
	enum o_document_value_type type;
};

struct o_property * o_property_new(char * name, enum o_document_value_type type)
{
	struct o_property * prop = o_malloc(sizeof(struct o_property *));
	prop->name = o_memdup(name, strlen(name));
	prop->type = type;
	return prop;
}

struct o_property * o_property_new_from_document(struct o_document *doc)
{
	char *name = o_document_value_get_string(o_document_field_get(doc, "name"));
	int type = o_document_value_get_int(o_document_field_get(doc, "type"));
	if (type == 11 || type == 14 || type == 15)
		type = 0;
	struct o_property * prop = o_property_new(name, type);

	return prop;
}

char * o_property_get_name(struct o_property * property)
{
	return property->name;
}

enum o_document_value_type o_property_get_type(struct o_property * property)
{
	return property->type;
}

void o_property_free(struct o_property * property)
{
	o_free(property->name);
	o_free(property);
}
