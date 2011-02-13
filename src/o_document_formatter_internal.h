#ifndef O_DOCUMENT_FORMATTER_INTERNAL_H_
#define O_DOCUMENT_FORMATTER_INTERNAL_H_
#include  "o_document_formatter.h"

struct o_document_formatter
{
	void (*o_document_formatter_serialize)(struct o_document_formatter * fm, struct o_document * doc, struct o_output_stream * output);
	void (*o_document_formatter_deserialize)(struct o_document_formatter * fm, struct o_document * doc, struct o_input_stream * input);
	void (*o_document_formatter_free)(struct o_document_formatter * fm);
};


#endif /* O_DOCUMENT_FORMATTER_INTERNAL_H_ */
