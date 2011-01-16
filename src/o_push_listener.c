#include "o_push_listener_internal.h"

void o_push_listener_push_record(struct o_push_listener * listener, struct o_record_id * id, struct o_raw_buffer * buff)
{
	listener->o_push_listner_push_record(listener, id, buff);
}

void o_push_listener_free(struct o_push_listener * listener)
{
	listener->o_push_listner_free(listener);
}
