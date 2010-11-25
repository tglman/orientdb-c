#ifndef O_INPUT_STREAM_INTERNAL_H_
#define O_INPUT_STREAM_INTERNAL_H_

#include "o_input_stream.h"

struct o_input_stream
{
	int (*o_input_stream_read)(struct o_input_stream * stream);
	int (*o_input_stream_peek)(struct o_input_stream * stream);
	int (*o_input_stream_read_bytes)(struct o_input_stream * stream, void * bytes, int size);
	void (*o_input_stream_free)(struct o_input_stream * stream);
};

#endif /* O_INPUT_STREAM_INTERNAL_H_ */
