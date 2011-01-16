#ifndef O_PUSH_LISTENER_INTERNAL_H_
#define O_PUSH_LISTENER_INTERNAL_H_
#include "o_push_listener.h"

struct o_push_listener
{
	void (*o_push_listner_push_record)(struct o_push_listener * listener, struct o_record_id * id, struct o_raw_buffer * buff);
	void (*o_push_listner_free)(struct o_push_listener * listener);
};

#endif /* O_PUSH_LISTENER_INTERNAL_H_ */
