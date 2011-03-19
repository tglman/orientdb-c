#ifndef O_DATABASE_DOCUMENT_H_
#define O_DATABASE_DOCUMENT_H_
#include "o_database_error_handler.h"
#include "o_document.h"
#include "o_metadata.h"
#define DOCUMENT_DB_TYPE 'd'

/** Represent an document database structure.
 *
 */
struct o_database_document;

/** create a new instance of a document database
 *
 * @param connection_url the database connection url.
 * @return the new document database instance.
 */
struct o_database_document * o_database_document_new(char * connection_url);

/** create a new instance of a document database and associate an error hendler
 *
 * @param connection_url the database connection url.
 * @param error_handler the handler of eventually errors.
 * @return the new database document instance.
 */
struct o_database_document * o_database_document_new_error_handler(char * connection_url, struct o_database_error_handler * error_handler);

/**Associate an error handler to database.
 *
 * @param db where associate error handler.
 * @param error_handler to associate.
 */
void o_database_document_reset_error_handler(struct o_database_document * db, struct o_database_error_handler *error_handler);

/** Open a connection to database with the specified user.
 *
 * @param db where open connection.
 * @param username the username with open the database.
 * @param password the password of specified username.
 */
void o_database_document_open(struct o_database_document * db, char * username, char * password);

/*! \brief Close the currente database instance.
 *
 * \param db to close.
 */
void o_database_document_close(struct o_database_document * db);

/**
 * \brief Save a document to the database.
 *
 * \param db where save.
 * \param doc to save.
 */
int o_database_document_save(struct o_database_document *db, struct o_document * doc, struct o_record_id **id);

/** \brief Delete an document from the database.
 *
 * \param db where delete.
 * \param doc to delete.
 */
void o_database_document_delete(struct o_database_document * db, struct o_document * doc);

/** Load a record from a id.
 *
 * @param db where load.
 * @param id of record to load.
 * @return loaded record
 */
struct o_document * o_database_document_load(struct o_database_document * db, struct o_record_id * id);

/*! \brief Retrieve the database metadata.
 *
 * \param db where retrieve metadata.
 * \return the metadata of databse.
 */
struct o_metadata * o_database_document_metadata(struct o_database_document * db);

/** Free the database structure with all data.
 *
 * @param db to free.
 */
void o_database_document_free(struct o_database_document * db);

#endif /* O_DATABASE_DOCUMENT_H_ */
