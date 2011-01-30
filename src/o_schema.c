#include "o_schema_internal.h"
#include "o_map_string.h"
#include "o_class_internal.h"
#include "o_memory.h"

struct o_schema
{
	struct o_map_string *classes;
};

struct o_schema * o_schema_new()
{
	struct o_schema *schema = o_malloc(sizeof(struct o_schema));
	schema->classes = o_map_string_new();
	return schema;
}
struct o_schema * o_schema_from_document(struct o_document * doc)
{
	struct o_schema * sc = o_schema_new();
	return sc;
}

struct o_class * o_schema_get_class(struct o_schema * schema, char * name)
{
	return (struct o_class *) o_map_string_get(schema->classes, name);
}

struct o_class * o_schema_create_class(struct o_schema * schema, char * name)
{
	struct o_class * cl = o_class_new(name);
	o_map_string_put(schema->classes, name, cl);
	return cl;
}

struct o_class ** o_schema_get_classes(struct o_schema * schema, int * n_classes)
{
	return (struct o_class **) o_map_string_values(schema->classes, n_classes);
}

int o_schema_exist_class(struct o_schema * schema, char * name)
{
	return o_map_string_get(schema->classes, name) != 0;
}

void o_schema_remove_class(struct o_schema * schema, char * name)
{
	struct o_class * cl = o_map_string_remove(schema->classes, name);
	o_class_free(cl);
}

void o_schema_free(struct o_schema * schema)
{
	int size;
	struct o_class ** cls = o_schema_get_classes(schema, &size);
	while (size > 0)
		o_class_free(cls[--size]);
	o_map_string_free(schema->classes);
	o_free(schema);
}
