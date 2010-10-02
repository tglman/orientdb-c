#ifndef O_STORAGE_REMOTE_H_
#define O_STORAGE_REMOTE_H_
#include "o_connection_remote.h"

struct o_storage_remote;

/**
 * Create a new instance of a remote storage.
 *
 * @param conn the remote connection.
 * @param error_handler the error handler.
 * @return the new storage instace.
 *
 */
struct o_storage * o_storage_remote_new(struct o_connection_remote * conn, struct o_database_error_handler * error_handler);

#endif /* O_STORAGE_REMOTE_H_ */
