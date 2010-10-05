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

/**
 * Open an storage on connection if exist.
 *
 * @param the name of storage.
 * @param username for current connection.
 * @param password correspondent to username.
 * @return the new instance of storage.
 */
struct o_storage * o_connection_storage_open(struct o_connection * conn,char * name,char * username, char * password);

/**
 * Free a connection structure and close all socket.
 *
 * @param connection to free.
 */
void o_connection_free(struct o_connection * connection);

#endif /* O_CONNECTION_H_ */
