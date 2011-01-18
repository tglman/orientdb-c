#include "o_class.h"
#include "o_schema.h"
#include "o_memory.h"
#include "o_map_string.h"
#include "o_property_internal.h"
#include <string.h>

struct o_class
{
	char * name;
	struct o_class * superclass;
	struct o_map_string * properties;
};

struct o_class * o_class_new(char * name)
{
	struct o_class * cl = o_malloc(sizeof(struct o_class));
	cl->name = o_memdup(name, strlen(name));
	cl->properties = o_map_string_new();
	return cl;
}

char * o_class_get_name(struct o_class * class)
{
	return class->name;
}

struct o_class * o_class_get_superclass(struct o_class * class)
{
	return class->superclass;
}

void o_class_set_superclass(struct o_class * class, struct o_class * superclass)
{
	class->superclass = superclass;
}

struct o_property * o_class_get_property(struct o_class * class, char * name)
{
	//TODO: find in superclass.
	return (struct o_property *) o_map_string_get(class->properties, name);
}

struct o_property * o_class_create_property(struct o_class * class, char * name, enum o_document_value_type type)
{
	struct o_property * prop = o_property_new(name, type);
	o_map_string_put(class->properties, name, prop);
	return prop;
}
