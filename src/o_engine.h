#ifndef O_ENGINE_H_
#define O_ENGINE_H_
#include "o_storage.h"
#include "o_connection.h"

/*! \brief Instanziate a new storage on a connection.
 *
 * \param connection_url the target connection of storage.
 * \param username the username of storage.
 * \param password the password of storage.
 * \return a new storage instance.
 */
struct o_storage * o_engine_get_storage(char * connection_url, char * username, char * password);

/*! \brief Release an connection on engine.
 *
 * \param connection to release.
 */
void o_engine_release_connection(struct o_connection * connection);

#endif /* O_ENGINE_H_ */
