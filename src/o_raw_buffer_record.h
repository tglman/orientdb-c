#ifndef O_RAW_BUFFER_RECORD_H_
#define O_RAW_BUFFER_RECORD_H_
#include "o_raw_buffer.h"
#include "o_record.h"

/*! \brief Create an row buffer on a record.
 *
 * \param record to wrap.
 * \return the new row buffer;
 */
struct o_raw_buffer * o_raw_buffer_record(struct o_record * record);

#endif /* O_RAW_BUFFER_RECORD_H_ */
