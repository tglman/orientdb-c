#ifndef O_RAW_BUFFER_BYTE_H_
#define O_RAW_BUFFER_BYTE_H_
#include "o_raw_buffer.h"

/*! \brief Create A Raw buffer from byte values.
 *
 * \param type of data
 * \param varsion number of version of data.
 * \param content the content.
 * \param content_size the size of content.
 */
struct o_raw_buffer * o_raw_buffer_byte(int type, int version, unsigned char * content, int content_size);

#endif /* O_RAW_BUFFER_BYTE_H_ */
