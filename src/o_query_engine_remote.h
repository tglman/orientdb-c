#ifndef O_QUERY_ENGINE_REMOTE_H_
#define O_QUERY_ENGINE_REMOTE_H_
#include "o_query_engine.h"

struct o_storage_remote;

/*! \brief Create A remote query engine on a connection.
 *
 * \param remote storage.
 * \return a new query engine.
 */
struct o_query_engine * o_query_engine_remote_new(struct o_storage_remote * remote);

#endif /* O_QUERY_ENGINE_REMOTE_H_ */
