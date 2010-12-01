#ifndef O_ROW_BUFFER_INTERNAL_H_
#define O_RAW_BUFFER_INTERNAL_H_
#include "o_raw_buffer.h"

struct o_raw_buffer
{
	int (*o_raw_buffer_content_size)(struct o_raw_buffer * buff);
	unsigned char * (*o_raw_buffer_content)(struct o_raw_buffer * buff, int * content_size);
	int (*o_raw_buffer_version)(struct o_raw_buffer * buff);
	char (*o_raw_buffer_type)(struct o_raw_buffer * buff);
	void (*o_raw_buffer_free)(struct o_raw_buffer * buff);
};

#endif /* O_RAW_BUFFER_INTERNAL_H_ */
