#include "o_record_factory.h"
#include "o_record.h"
#include "o_record_raw.h"
#include "o_document.h"
#include "o_exceptions.h"

struct o_record * o_record_factory(char record_type)
{
	switch (record_type)
	{
	case RAW_RECORD_TYPE:
		return o_record_raw_new();
	case DOCUMENT_RECORD_TYPE:
		return o_document_o_record(o_document_new());
	}
	throw(o_exception_new("Unsupported record type",20));
	//Never executed only remove warning.
	return 0;
}
