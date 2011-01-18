#include "o_schema_internal.h"
#include "o_map_string.h"
#include "o_class_internal.h"

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

struct o_schema * o_schema_load(struct o_database * db)
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
