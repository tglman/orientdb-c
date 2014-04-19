#include "o_class.h"
#include "o_schema.h"
#include "o_memory.h"
#include "o_map_string.h"
#include "o_property_internal.h"
#include "o_document.h"
#include <string.h>

struct o_class
{
	int id;
	char * name;
	char * shortName;
	int overSize;
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
	cl->name = o_memdup(name, strlen(name) + 1);
	cl->properties = o_map_string_new();
	cl->shortName = 0;
	cl->overSize = 0;
	return cl;
}

char * o_class_get_name(struct o_class * oclass)
{
	return oclass->name;
}

struct o_class * o_class_get_superclass(struct o_class * oclass)
{
	return oclass->superclass;
}

void o_class_set_superclass(struct o_class * oclass, struct o_class * superclass)
{
	oclass->superclass = superclass;
}

struct o_property * o_class_get_property(struct o_class * oclass, char * property_name)
{
	struct o_property * prop;
	do
	{
		prop = (struct o_property *) o_map_string_get(oclass->properties, property_name);
		oclass = oclass->superclass;
	} while (prop == 0 && oclass != 0);
	return prop;
}

struct o_property * o_class_create_property(struct o_class * oclass, char * property_name, enum o_document_value_type type)
{
	struct o_property * prop = o_property_new(property_name, type);
	o_map_string_put(oclass->properties, property_name, prop);
	return prop;
}

int o_class_exist_property(struct o_class * oclass, char * property_name)
{
	return o_map_string_get(oclass->properties, property_name) != 0;
}

void o_class_remove_property(struct o_class * oclass, char * property_name)
{
	struct o_property * pro = (struct o_property *) o_map_string_remove(oclass->properties, property_name);
	o_free(pro);
}

const int * o_class_get_clusters(struct o_class * oclass, int * n_cluster)
{
	*n_cluster = oclass->n_clusterIds;
	return oclass->clusterIds;
}

void o_class_add_cluster(struct o_class * oclass, int clusterId)
{
	oclass->n_clusterIds++;
	oclass->clusterIds = o_realloc(oclass->clusterIds, (oclass->n_clusterIds) * sizeof(int));
	oclass->clusterIds[oclass->n_clusterIds - 1] = clusterId;
}

struct o_class * o_class_new_from_document(struct o_document * doc)
{
	char * name = o_document_value_get_string(o_document_field_get(doc, "name"));
	struct o_class * cl = o_class_new(name);
	//cl->id = o_document_value_get_int(o_document_field_get(doc, "id"));
	struct o_document_value * shortName = o_document_field_get(doc, "shortName");
	if (shortName != 0)
		cl->shortName = strdup( o_document_value_get_string(shortName));
	cl->defaultClusterId = o_document_value_get_int(o_document_field_get(doc, "defaultClusterId"));
	struct o_document_value * overSizeValue = o_document_field_get(doc, "overSize");
	if (overSizeValue != 0)
		cl->overSize = o_document_value_get_int(overSizeValue);
	struct o_document_value ** values = o_document_value_get_array(o_document_field_get(doc, "clusterIds"), &cl->n_clusterIds);
	cl->clusterIds = o_malloc(sizeof(int) * (cl->n_clusterIds));
	int i = cl->n_clusterIds;
	while (i > 0)
	{
		--i;
		cl->clusterIds[i] = o_document_value_get_int(values[i]);
	}

	int n_properties;
	struct o_document_value ** properties = o_document_value_get_array(o_document_field_get(doc, "properties"), &n_properties);
	while (n_properties > 0)
	{
		--n_properties;
		struct o_property * prop = o_property_new_from_document(o_document_value_get_embedded(properties[n_properties]));
		o_map_string_put(cl->properties, o_property_get_name(prop), prop);
	}

	return cl;
}

struct o_property ** o_class_properties(struct o_class * oclass, int * n_properties)
{
	return (struct o_property **) o_map_string_values(oclass->properties, n_properties);
}

int o_class_get_id(struct o_class * oclass)
{
	return oclass->id;
}

void o_class_set_id(struct o_class * oclass, int id)
{
	oclass->id = id;
}

int o_class_get_default_cluster_id(struct o_class * oclass)
{
	return oclass->defaultClusterId;
}

void o_class_set_default_cluster_id(struct o_class * oclass, int cluster_id)
{
	oclass->defaultClusterId = cluster_id;
}

void o_class_free(struct o_class * oclass)
{
	int size;
	struct o_property ** ps = o_class_properties(oclass, &size);
	while (size > 0)
		o_property_free(ps[--size]);
	o_map_string_free(oclass->properties);
	if (oclass->shortName != 0)
		o_free(oclass->shortName);
	o_free(oclass->clusterIds);
	o_free(oclass->name);
	o_free(oclass);
}
