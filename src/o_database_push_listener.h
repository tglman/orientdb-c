#ifndef O_DATABASE_PUSH_LISTENER_H_
#define O_DATABASE_PUSH_LISTENER_H_
#include "o_push_listener.h"
#include "o_database.h"

/*! \brief Create a database push listener,
 *
 * \param db the database owner of listener.
 * \param the new listener.
 */
struct o_push_listener* o_database_push_listener_new(struct o_database * db);

#endif /* O_DATABASE_PUSH_LISTENER_H_ */
