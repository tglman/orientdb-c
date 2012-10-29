#include "o_document_formatter_internal.h"
#include "o_document_formatter_standard.h"

void o_document_formatter_serialize(struct o_document_formatter * fm, struct o_document * doc, struct o_output_stream * output)
{
	fm->o_document_formatter_serialize(fm, doc, output);
}

void o_document_formatter_deserialize(struct o_document_formatter * fm, struct o_document * doc, struct o_input_stream * input)
{
	fm->o_document_formatter_deserialize(fm, doc, input);
}

void o_document_formatter_free(struct o_document_formatter * fm)
{
	fm->o_document_formatter_free(fm);
}

struct o_document_formatter * default_formatter = 0;

struct o_document_formatter * o_document_formatter_factory_default()
{
	if (default_formatter == 0)
	{
		default_formatter = o_document_formatter_standard_default();
	}
	return default_formatter;
}
