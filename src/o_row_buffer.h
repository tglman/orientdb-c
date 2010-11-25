#ifndef O_ROW_BUFFER_H_
#define O_ROW_BUFFER_H_

struct o_raw_buffer;

/*! \brief Retrieve the content buffer size.
 *
 * \param the raw_buffer where retrieve size.
 * \return the size of buffer.
 */
int o_raw_buffer_content_size(struct o_raw_buffer * buff);

/*! \brief Retrieve the content of buffer.
 *
 * \param buff where read.
 * \param content_size readed.
 * \return the content of buffer.
 */
char * o_raw_buffer_content(struct o_raw_buffer * buff, int * content_size);

/*! \brief Retrieve the version of content in buffer.
 *
 * \return int the version.
 */
int o_raw_buffer_version(struct o_raw_buffer * buff);

/*! \brief Retrieve the type of value in the buffer.
 *
 * \param buff where read.
 * \return the type of content in buffer.
 */
char o_raw_buffer_type(struct o_raw_buffer * buff);

#endif /* O_ROW_BUFFER_H_ */
