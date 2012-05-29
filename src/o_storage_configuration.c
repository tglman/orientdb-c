#include "o_storage_configuration.h"
#include "o_memory.h"
#include <string.h>

char * o_storage_next_entry(unsigned char * content, int *cursor, int buff_size)
{
	int start = *cursor;
	while (content[(*cursor)++] != '|' && *cursor < buff_size)
		;
	char * retval = o_memdup(content + start, *cursor - start);
	retval[*cursor - start - 1] = 0;
	return retval;
}

struct o_storage_configuration
{
	int version;
	char * name;
	struct o_record_id *schema;
	struct o_record_id *dictionary;
};

struct o_record_id * record_id_from_string(char * id)
{
	if (*id == '#')
		id++;
	char* pos = strchr(id, ':');
	if (pos == 0)
		return 0;
	*pos = 0;
	pos++;
	return o_record_id_new(atoi(id), atoi(pos));
}

struct o_storage_configuration * o_storage_configuration_load(struct o_raw_buffer * buff)
{
	struct o_storage_configuration * conf = o_malloc(sizeof(struct o_storage_configuration));
	int buff_size;
	unsigned char * content = o_raw_buffer_content(buff, &buff_size);
	int cursor = 0;
	char * val = o_storage_next_entry(content, &cursor, buff_size);
	conf->version = atoi(val);
	o_free(val);
	conf->name = o_storage_next_entry(content, &cursor, buff_size);
	val = o_storage_next_entry(content, &cursor, buff_size);
	conf->schema = record_id_from_string(val);
	o_free(val);
	val = o_storage_next_entry(content, &cursor, buff_size);
	conf->dictionary = record_id_from_string(val);
	o_free(val);
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

