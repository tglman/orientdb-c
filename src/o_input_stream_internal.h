#ifndef O_INPUT_STREAM_INTERNAL_H_
#define O_INPUT_STREAM_INTERNAL_H_

#include "o_input_stream.h"

struct o_input_stream
{
	int peek_buffer;
	int (*o_input_stream_read_bytes)(struct o_input_stream * stream, void * bytes, int size);
	void (*o_input_stream_free)(struct o_input_stream * stream);
};

/*! \brief Initialize the input stream with default value.
 *
 * \param input to init.
 */
void o_input_stream_internal_new(struct o_input_stream * input);

#endif /* O_INPUT_STREAM_INTERNAL_H_ */
