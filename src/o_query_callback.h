#ifndef O_QUERY_CALLBACK_H_
#define O_QUERY_CALLBACK_H_
#include "o_record_id.h"
#include "o_raw_buffer.h"

/*! \brief The callback to give record from backend.
 *
 * \param add_info custom info give with callback.
 * \param id current record id.
 * \param buferr current record data.
 */
typedef void (*query_result_callback)(void * add_info, struct o_record_id *id, struct o_raw_buffer * buffer);

#endif /* O_QUERY_CALLBACK_H_ */
