#include "o_database_push_listener.h"
#include "o_push_listener_internal.h"
#include "o_database_internal.h"
#include "o_memory.h"

struct o_database_push_listener
{
	struct o_push_listener listener;
	struct o_database * db;
};

void o_database_push_listener_push_record(struct o_push_listener * listener, struct o_record_id * id, struct o_raw_buffer * buff)
{
	struct o_database_push_listener * dbl = (struct o_database_push_listener *) listener;
	if (dbl->db != 0)
		o_database_record_from_content(dbl->db, id, buff);
}

void o_database_push_listener_free(struct o_push_listener * listener)
{
	struct o_database_push_listener * dbl = (struct o_database_push_listener *) listener;
	if (dbl->db != 0)
		o_database_remove_referrer(dbl->db, &dbl->db);
	o_free(listener);
}

struct o_push_listener* o_database_push_listener_new(struct o_database * db)
{
	struct o_database_push_listener * listener = o_malloc(sizeof(struct o_database_push_listener));
	listener->db = db;
	listener->listener.o_push_listner_free = o_database_push_listener_free;
	listener->listener.o_push_listner_push_record = o_database_push_listener_push_record;
	o_database_add_referrer(db, &listener->db);
	return &listener->listener;
}
