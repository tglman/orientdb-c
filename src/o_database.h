#ifndef O_DATABASE_H_
#define O_DATABASE_H_
#include "o_database_error_handler.h"
#include "o_record.h"
#include "o_list_record.h"
#include "o_query.h"

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
 * \return 1 if operation is run correctly otherwise 0;
 */
int o_database_save(struct o_database * db, struct o_record * record, struct o_record_id ** rid);

/*! Save an record to database.
 *
 * \param db where save
 * \param record to save.
 * \param cluster_name the name of cluster where save.
 * \return 1 if operation is run correctly otherwise 0.
 */
int o_database_save_cluster(struct o_database * db, struct o_record * record, char * cluster_name, struct o_record_id ** rid);

/*! \brief Delete an record from the database.
 *
 * \param db where delete.
 * \param record to delete.
 * \return 1 if operation is run correctly otherwise 0.
 */
int o_database_delete(struct o_database * db, struct o_record * record);

/*! \brief Retrieve a record from the id.
 *
 * \param db the database.
 * \param rid the record identifier.
 * \return the persistent record.
 */
struct o_record * o_database_load(struct o_database * db, struct o_record_id * rid);

/*! \brief make a query on the database.
 *
 * \param where execute the query.
 * \param query to execute.
 * \return the list of result.
 */
struct o_list_record * o_database_query(struct o_database * db, struct o_query * query);

/*! \brief create a new raw record and associate it to this database.
 *
 * \param db to associate.
 * \return the new record.
 */
struct o_record * o_database_record_new(struct o_database * db);

/*! \brief create a new empty record of specified type and associate it to this database.
 *
 * \param database to associate.
 * \param type of record must be created.
 * \return the new empty record created.
 */
struct o_record * o_database_record_new_type(struct o_database * db, char type);

/*! \brief Retrieve the db from the current context.
 *
 * \return the database in current context.
 */
struct o_database * o_database_context_database();

/** Close the current database session
 *
 * @param db to close.
 */
void o_database_close(struct o_database * db);

/*! \brief retrieve the metadata of database.
 *
 * \param db to retrieve metadata.
 * \param record that describe metadata.
 */
struct o_record * o_database_metadata(struct o_database * db);

/** Free the database structure with all data.
 *
 * @param db to free.
 */
void o_database_free(struct o_database * db);

#endif /* O_DATABASE_H_ */
