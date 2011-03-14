#ifndef O_OUTPUT_STREAM_DATA_H_
#define O_OUTPUT_STREAM_DATA_H_
#include "o_output_stream.h"

struct o_output_stream_data;

/*! \brief Create a new data output stream on an output stream
 *
 * \param stream to wrap.
 * \return a new instance of data output stream
 */
struct o_output_stream_data * o_output_stream_data_new(struct o_output_stream *stream);

/*! \brief write a string on the output stream
 *
 * \param data_stream to write.
 * \param string to write.
 */
void o_output_stream_data_write_string(struct o_output_stream_data *data_stream, char * string);

/*! \brief write a byte value.
 *
 * \param data_stream where write.
 * \param bvalue to write.
 */
void o_output_stream_data_write_byte(struct o_output_stream_data *data_stream, char bvalue);

/*! \brief write a boolean value.
 *
 * \param data_stream where write.
 * \param bvalue to write.
 */
void o_output_stream_data_write_bool(struct o_output_stream_data *data_stream, int bvalue);

/*! \brief write a int value.
 *
 * \param data_stream where write.
 * \param bvalue to write.
 */
void o_output_stream_data_write_int(struct o_output_stream_data *data_stream, int ivalue);

/*! \brief write a short value.
 *
 * \param data_stream where write.
 * \param bvalue to write.
 */
void o_output_stream_data_write_short(struct o_output_stream_data *data_stream, short svalue);

/*! \brief write a long value.
 *
 * \param data_stream where write.
 * \param bvalue to write.
 */
void o_output_stream_data_write_long(struct o_output_stream_data *data_stream, long long lvalue);

/*! \brief Free an data output stream.
 *
 * \param data_stream to free.
 */
void o_output_stream_data_free(struct o_output_stream_data *data_stream);

#endif /* O_OUTPUT_STREAM_DATA_H_ */
