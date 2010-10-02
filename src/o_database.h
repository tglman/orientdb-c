#ifndef O_DATABASE_H_
#define O_DATABASE_H_

#include "o_database_error_handler.h"

/**
 * Rapresent a database.
 */
struct o_database;

/** create a new instance of a database
 *
 * @param connection_url the database connection url.
 * @return the new database instance.
 */
struct o_database * o_database_new(char * connection_url);

/** create a new instance of a database and associate an error hendler
 *
 * @param connection_url the database connection url.
 * @param error_handler the handler of eventually errors.
 * @return the new database instance.
 */
struct o_database * o_database_new_error_handler(char * connection_url, struct o_database_error_handler * error_handler);

/**Associate an error handler to database.
 *
 * @param db where associate error handler.
 * @param error_handler to associate.
 */
void o_database_reset_error_handler(struct o_database * db, struct o_database_error_handler *error_handler);

/** Open a connection to database with the specified user.
 *
 * @param db where open connection.
 * @param username the username with open the database.
 * @param password the password of specified username.
 */
void o_database_open(struct o_database * db, char * username, char * password);

/** Free the database structure with all data.
 *
 * @param db to free.
 */
void o_database_free(struct o_database * db);

#endif /* O_DATABASE_H_ */
