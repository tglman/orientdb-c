#ifndef O_STRING_BUFFER_H_
#define O_STRING_BUFFER_H_

/*! \brief A buffer of string.
 *
 */
struct o_string_buffer;

/*! \brief The buffer constructor.
 *
 * \return the new buffer instance.
 */
struct o_string_buffer * o_string_buffer_new();


/*! \brief Append a string to buffer.
 *
 * \param buff where append string.
 * \param to_append the string to append.
 */
void o_string_buffer_append(struct o_string_buffer * buff, char * to_append);

/*! \brief Append an long integer to buffer.
 *
 * \param buff where append.
 * \param long_val to append.
 */
void o_string_buffer_append_long(struct o_string_buffer * buff, long long long_val);

/*! \brief Append double value to buffer.
 *
 * \param buff where append.
 * \param double_val to append.
 */
void o_string_buffer_append_double(struct o_string_buffer * buff, double double_val);

/*! \brief Append an character to the buffer.
 *
 * \param buff where append.
 * \parma ch to append.
 */
void o_string_buffer_append_char(struct o_string_buffer * buff, char ch);

/*! \brief Clear the content from the buffer.
 *
 * \param buff to clear.
 */
void o_string_buffer_clear(struct o_string_buffer * buff);

/*! \brief Retrieve the global str from buffer.
 *
 * \param buff buffer to transform in string.
 * \return the global string in buffer in a new allocated memory.
 */
char * o_string_buffer_str(struct o_string_buffer * buff);

/*! \brief Free the buffer memory and delete it.
 *
 * \param toFree to free.
 */
void o_string_buffer_free(struct o_string_buffer * to_free);

#endif /* O_STRING_BUFFER_H_ */
