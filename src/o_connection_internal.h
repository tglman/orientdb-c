#ifndef O_CONNECTION_INTERNAL_H_
#define O_CONNECTION_INTERNAL_H_
#include "o_connection.h"

struct o_connection
{
	struct o_database_error_handler * error_handler;
	enum o_url_type type;
	void (*free)(struct o_connection *conn);
	struct o_storage * (*storage_open)(struct o_connection *conn,char * storage_name);
};

/** Create a new remote connection.
 *
 * @param host of remote connection.
 * @param port of remote connection.
 * @param username of remote connection.
 * @param error_handler for eventually error occurs.
 *
 * @return the new connection instance otherwise 0 if error occur.
 */
struct o_connection * o_connection_remote_new(char * host, int port, char * username, char * password,
		struct o_database_error_handler * error_handler);

/** Create a new http connection.
 *
 * @param host of http connection.
 * @param port of http connection.
 * @param username of http connection.
 * @param error_handler for eventually error occurs.
 *
 * @return the new connection instance otherwise 0 if error occur.
 */
struct o_connection * o_connection_http_new(char * host, int port, char * username, char * password, struct o_database_error_handler * error_handler);

#endif /* O_CONNECTION_INTERNAL_H_ */
