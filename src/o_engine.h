#ifndef O_ENGINE_H_
#define O_ENGINE_H_
#include "o_storage.h"

/*! \brief The main engine structure.
 *
 */
struct o_engine;

/*! \brief Retrieve the main engine structure and increment reference count.
 *
 * \return the main engine reference.
 */
struct o_engine * o_engine_get_instance();

/*! \brief Instanziate a new storage on a connection.
 *
 * \param connection_url the target connection of storage.
 * \param username the username of storage.
 * \param password the password of storage.
 * \return a new storage instance.
 */
struct o_storage * o_engine_get_storage(char * connection_url, char * username, char * password);

/*! \brief Release the main engine reference (count--).
 *
 * \param engine to release.
 */
void o_engine_release();

#endif /* O_ENGINE_H_ */
