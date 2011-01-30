#include "o_metadata_internal.h"
#include "o_memory.h"
#include "o_schema_internal.h"

struct o_metadata
{
	struct o_schema * schema;
};

struct o_schema * o_metadata_get_schema(struct o_metadata * metadata)
{
	return metadata->schema;
}

struct o_metadata * o_metadata_from_document(struct o_document *doc)
{
	struct o_metadata * meta = o_malloc(sizeof(struct o_metadata));
	struct o_document_value * schema = o_document_field_get(doc, "schema");
	meta->schema = o_schema_from_document((struct o_document *) o_document_value_get_link(schema));
	return meta;
}

void o_metadata_free(struct o_metadata * metadata)
{
	o_schema_free(metadata->schema);
	o_free(metadata);
}
