#ifndef O_DATABASE_H_
#define O_DATABASE_H_

#include "o_database_error_handler.h"
#include "o_record.h"

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

/*! Save an record to database.
 *
 * \param db where save
 * \param record to save.
 */
struct o_record_id * o_database_save(struct o_database * db, struct o_record * record);

/*! Save an record to database.
 *
 * \param db where save
 * \param record to save.
 * \param cluster_name the name of cluster where save.
 */
struct o_record_id * o_database_save_cluster(struct o_database * db, struct o_record * record, char * cluster_name);

/*! \brief Delete an record from the database.
 *
 * \param db where delete.
 * \param record to delete.
 */
void o_database_delete(struct o_database * db, struct o_record * record);

/*! \brief Retrieve a record from the id.
 *
 * \param db the database.
 * \param rid the record identifier.
 * \return the persistent record.
 */
struct o_record * o_database_load(struct o_database * db, struct o_record_id * rid);

/*! \brief create a new raw record.
 *
 * \return the new record.
 */
struct o_record * o_database_record_new();

/** Close the current database session
 *
 * @param db to close.
 */
void o_database_close(struct o_database * db);

/** Free the database structure with all data.
 *
 * @param db to free.
 */
void o_database_free(struct o_database * db);

#endif /* O_DATABASE_H_ */
