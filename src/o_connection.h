#ifndef O_CONNECTION_H_
#define O_CONNECTION_H_
#include "o_database_error_handler.h"
#include "o_url_resolver.h"
#include "o_storage.h"
/**
 * The database connection.
 */
struct o_connection;

/**
 * Create a new connection to database.
 * @param type of connection.
 * @param path the location for connection, or a file or an host:port string.
 * @return a new connection or 0 if connectio fail.
 *
 */
struct o_connection * o_connection_new(enum o_url_type type, char * path);

/** Retrieve a path from a connection.
 *
 * \param connection where retrieve the path.
 * \return the path of current connection.
 */
char * o_connection_get_path(struct o_connection * connection);

/** Retrieve the last user on the connection.
 *
 * \param connection where retrieve the user.
 * \return the last user of current connection.
 */
char * o_connection_get_last_user(struct o_connection * connection);

/**
 * Open an storage on connection if exist.
 *
 * @param the name of storage.
 * @param username for current connection.
 * @param password correspondent to username.
 * @return the new instance of storage.
 */
struct o_storage * o_connection_storage_open(struct o_connection * conn, char * name, char * username, char * password);

/*! \brief release an storage to the connection.
 *
 * \param conn where release.
 * \param storage where release.
 */
void o_connection_storage_release(struct o_connection * conn, struct o_storage * storage);

/**
 * Free a connection structure and close all socket.
 *
 * @param connection to free.
 */
void o_connection_free(struct o_connection * connection);

#endif /* O_CONNECTION_H_ */
