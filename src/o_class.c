#include "o_class.h"
#include "o_schema.h"
#include "o_memory.h"
#include "o_map_string.h"
#include "o_property_internal.h"
#include <string.h>

struct o_class
{
	int id;
	char * name;
	struct o_class * superclass;
	struct o_map_string * properties;
	int defaultClusterId;
	int *clusterIds;
	int n_clusterIds;
};

struct o_class * o_class_new(char * name)
{
	struct o_class * cl = o_malloc(sizeof(struct o_class));
	memset(cl, 0, sizeof(struct o_class));
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

struct o_property * o_class_get_property(struct o_class * class, char * property_name)
{
	struct o_property * prop;
	do
	{
		prop = (struct o_property *) o_map_string_get(class->properties, property_name);
		class = class->superclass;
	} while (prop == 0 && class != 0);
	return prop;
}

struct o_property * o_class_create_property(struct o_class * class, char * property_name, enum o_document_value_type type)
{
	struct o_property * prop = o_property_new(property_name, type);
	o_map_string_put(class->properties, property_name, prop);
	return prop;
}

int o_class_exist_property(struct o_class * class, char * property_name)
{
	return o_map_string_get(class->properties, property_name) != 0;
}

void o_class_remove_property(struct o_class * class, char * property_name)
{
	struct o_property * pro = (struct o_property *) o_map_string_remove(class->properties, property_name);
	o_free(pro);
}

const int * o_class_get_clusters(struct o_class * class, int * n_cluster)
{
	*n_cluster = class->n_clusterIds;
	return class->clusterIds;
}

void o_class_add_cluster(struct o_class * class, int clusterId)
{
	class->n_clusterIds++;
	class->clusterIds = o_realloc(class->clusterIds, (class->n_clusterIds) * sizeof(int));
	class->clusterIds[class->n_clusterIds - 1] = clusterId;
}

struct o_class * o_class_new_from_document(struct o_document * doc)
{
	char * name = o_document_value_get_string(o_document_field_get(doc, "name"));
	struct o_class * cl = o_class_new(name);
	cl->id = o_document_value_get_int(o_document_field_get(doc, "id"));
	cl->defaultClusterId = o_document_value_get_int(o_document_field_get(doc, "defaultClusterId"));

	struct o_document_value ** values = o_document_value_get_array(o_document_field_get(doc, "clusterIds"), &cl->n_clusterIds);
	cl->clusterIds = o_malloc(sizeof(int) * (cl->n_clusterIds));
	int i = cl->n_clusterIds;
	while (i > 0)
	{
		--i;
		cl ->clusterIds[i] = o_document_value_get_int(values[i]);
	}

	int n_properties;
	struct o_document_value ** properties = o_document_value_get_array(o_document_field_get(doc, "properties"), &n_properties);
	cl->clusterIds = o_malloc(sizeof(int) * (cl->n_clusterIds));
	while (n_properties > 0)
	{
		--n_properties;
		struct o_property * prop = o_property_new_from_document(o_document_value_get_embedded(properties[n_properties]));
		o_map_string_put(cl->properties, o_property_get_name(prop), prop);
	}

	return cl;
}

struct o_property ** o_class_properties(struct o_class * class, int * n_properties)
{
	return (struct o_property **) o_map_string_values(class->properties, n_properties);
}

int o_class_get_id(struct o_class * class)
{
	return class->id;
}

void o_class_set_id(struct o_class * class, int id)
{
	class->id = id;
}

int o_class_get_default_cluster_id(struct o_class * class)
{
	return class->defaultClusterId;
}

void o_class_set_default_cluster_id(struct o_class * class, int cluster_id)
{
	class->defaultClusterId = cluster_id;
}

void o_class_free(struct o_class * class)
{
	int size;
	struct o_property ** ps = o_class_properties(class, &size);
	while (size > 0)
		o_free(ps[--size]);
	o_map_string_free(class->properties);
	o_free(class->name);
	o_free(class);
}
