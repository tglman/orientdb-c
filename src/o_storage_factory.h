#ifndef O_STORAGE_FACTORY_H_
#define O_STORAGE_FACTORY_H_
#include "o_storage.h"
#include "o_url_resolver.h"

struct o_storage_factory;

/*! \brief Create a new storage factory for an url.
 *
 * \param type the type of connection.
 * \param path to open the storage.
 * \return the new storage factory.
 */
struct o_storage_factory * o_storage_factory_new(enum o_url_type type, char * path);

/*! \brief Open a storage on a db with an user and a password.
 *
 * \param factory object.
 * \param db_name to open.
 * \param user the username for open database.
 * \param passwd the password for open database.
 * \return the stoage istance.
 */
struct o_storage * o_storage_factory_storage_open(struct o_storage_factory * factory, char * db_name, char * user, char * passwd);

/*! \brief Retrieve the type of current factory.
 *
 * \param factory where retrieve the type
 * \return the type of factory.
 */
enum o_url_type o_storage_factory_get_type(struct o_storage_factory * factory);

/*! \brief Retrieve the path for the current storage factory.
 *
 * \param factory of storage.
 * \return the current path.
 */
char * o_storage_factory_get_path(struct o_storage_factory * factory);

/*! \brief Destroy an storage factory.
 *
 * \param factory to free.
 */
void o_storage_factory_free(struct o_storage_factory *factory);

#endif /* O_STORAGE_FACTORY_H_ */
