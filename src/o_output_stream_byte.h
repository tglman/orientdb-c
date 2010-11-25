#ifndef O_OUTPUT_STREAM_BYTE_H_
#define O_OUTPUT_STREAM_BYTE_H_
#include "o_output_stream.h"

/*! \brief Create an byte buffer output stream.
 *
 * \return the new output_stream
 */
struct o_output_stream * o_output_stream_byte_buffer();

/*! \brief retrieve the content from the output stream.
 *
 * \param stream where read.
 * \param content_size filled whith the number of byte writed.
 * \return the content of stream;
 */
char * o_output_stream_byte_content(struct o_output_stream * stream, int * content_size);

#endif /* O_OUTPUT_STREAM_BYTE_H_ */
