#ifndef O_RECORD_CONTENT_C_
#define O_RECORD_CONTENT_C_
#include "o_memory.h"

struct o_record_content
{

};

struct o_record_content * o_record_content_new()
{
	struct o_record_content * content = o_malloc(sizeof(struct o_record_content));
	return content;
}

char* o_record_content_bytes(struct o_record_content * content)
{
	//TODO:
	return 0;
}

int o_record_content_bytes_length(struct o_record_content * content)
{
	//TODO:
	return 0;
}

char o_record_content_type(struct o_record_content * content)
{
	//TODO:
	return 0;
}

void o_record_content_free(struct o_record_content * content)
{
	o_free(content);
}

#endif /* O_RECORD_CONTENT_C_ */
