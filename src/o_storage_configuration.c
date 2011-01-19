#include "o_storage_configuration.h"
#include "o_memory.h"

struct o_storage_configuration
{
	struct o_record_id *schema;
	struct o_record_id *dictionary;
};

struct o_storage_configuration * o_storage_configuration_load(struct o_storage * storage)
{
	struct o_storage_configuration * conf = o_malloc(sizeof(struct o_storage_configuration));
	//TODO:load configuration from storage.
	return conf;
}

struct o_record_id * o_storage_configuration_get_schema(struct o_storage_configuration * conf)
{
	return conf->schema;
}

struct o_record_id * o_storage_configuration_get_dictionary(struct o_storage_configuration * conf)
{
	return conf->schema;
}
