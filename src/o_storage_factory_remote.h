#ifndef O_STORAGE_FACTORY_REMOTE_H_
#define O_STORAGE_FACTORY_REMOTE_H_

#include "o_connection_remote.h"

struct o_storage_factory_remote;

/*! \brief Create an factory for remote storage protocol.
 *
 * \param host where connect
 * \return a new storage factory remote.
 */
struct o_storage_factory * o_storage_factory_remote_new(char * host);

/*!\brief Retrieve a connection remote for write.
 *
 * \param factory where acquire the connection.
 * \return the connection where operate.
 */
struct o_connection_remote * o_storage_factory_remote_begin_write(struct o_storage_factory_remote * factory);

/*! \brief Release a remote  write connection to the factory.
 *
 * \param factory where release the connection.
 * \param conn to release.
 */
void o_storage_factory_remote_end_write(struct o_storage_factory_remote * factory, struct o_connection_remote * conn);

/*! \brief Start an read session from the factory.
 *
 * \param factory where begin a read operation.
 * \param session_id the session id to read.
 * \param returnCode filled with the status of comand returned from server.
 * \return the connection correspondent to the read operation
 */
struct o_connection_remote * o_storage_factory_remote_begin_read(struct o_storage_factory_remote * factory, int session_id, int *returnCode);

/*! \brief Release an on read connection.
 *
 * \param factory to release the connection.
 * \param conn to release.
 */
void o_storage_factory_remote_end_read(struct o_storage_factory_remote * factory, struct o_connection_remote * conn);

#endif /* O_STORAGE_FACTORY_REMOTE_H_ */
