#ifndef O_PUSH_LISTENER_H_
#define O_PUSH_LISTENER_H_
#include "o_record_id.h"
#include "o_raw_buffer.h"

struct o_push_listener;

/*! \brief sent a push record event to listener.
 *
 * \param listener where notify.
 * \param id of notified record.
 * \param buff content received.
 */
void o_push_listener_push_record(struct o_push_listener * listener, struct o_record_id * id, struct o_raw_buffer * buff);

/*! \brief free the listener structure.
 *
 * \param listener to free.
 */
void o_push_listener_free(struct o_push_listener * listener);

#endif /* O_PUSH_LISTENER_H_ */
