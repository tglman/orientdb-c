#ifndef O_INPUT_STREAM_H_
#define O_INPUT_STREAM_H_

struct o_input_stream;

/*! \brief Crete an input stream from an byte array.
 *
 * \param bytes content.
 * \param lenght the lenght of content.
 */
struct o_input_stream * o_input_stream_new_bytes(char * bytes, int lenght);

/*! \brief Read an byte from stream
 *
 * \param stream where read.
 * \return the value readed.
 */
int o_input_stream_read(struct o_input_stream * stream);

/*! \brief the Input stream to free.
 *
 * \param to_free stream to free.
 */
void o_input_stream_free(struct o_input_stream * to_free);

#endif /* O_INPUT_STREAM_H_ */
