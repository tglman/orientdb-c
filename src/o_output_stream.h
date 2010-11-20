#ifndef O_OUTPUT_STREAM_H_
#define O_OUTPUT_STREAM_H_

struct o_output_stream;

/*! \brief Write An byte on output stream.
 *
 * \param stream where write.
 * \param c to write.
 */
void o_output_stream_write(struct o_output_stream * stream, char c);

/*! \brief write more than bytes to stream.
 *
 * \param stream where write.
 * \param bytes to write.
 * \param nbytes number of bytes to write.
 */
void o_output_stream_write_bytes(struct o_output_stream * stream, void *bytes, int nbytes);

/*! \brief Flush all buffer of stream.
 *
 * \param strea to flush.
 */
void o_output_stream_flush(struct o_output_stream * stream);

/*! \brief free an output stream.
 *
 * \param stream to free.
 */
void o_output_stream_free(struct o_output_stream * stream);

#endif /* O_OUTPUT_STREAM_H_ */
