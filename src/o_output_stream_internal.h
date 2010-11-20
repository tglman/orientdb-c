#ifndef O_OUTPUT_STREAM_INTERNAL_H_
#define O_OUTPUT_STREAM_INTERNAL_H_
#include "o_output_stream.h"

struct o_output_stream
{
	void (*o_output_stream_write)(struct o_output_stream * stream, char c);
	void (*o_output_stream_write_bytes)(struct o_output_stream * stream, void *bytes, int nbytes);
	void (*o_output_stream_flush)(struct o_output_stream * stream);
	void (*o_output_stream_free)(struct o_output_stream * stream);
};

#endif /* O_OUTPUT_STREAM_INTERNAL_H_ */
