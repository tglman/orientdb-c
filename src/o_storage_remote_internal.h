#ifndef O_STORAGE_REMOTE_INTERNAL_H_
#define O_STORAGE_REMOTE_INTERNAL_H_
#include "o_storage_internal.h"
#include "o_storage_remote.h"
#include "o_connection_remote.h"

struct o_storage_remote_cluster
{
	char * storage_name;
	int id;
	char * type;
	struct o_storage_remote_cluster *next;
};

struct o_storage_remote
{
	struct o_storage storage;
	struct o_storage_factory_remote * storage_factory;
	struct o_native_lock * exclusive_lock;
	int session_id;
	int n_cluster;
	struct o_storage_remote_cluster *clusters;
	int default_cluster_id;
};

/*! \brief Manage of start of end of request with error management.
 *
 * \param storage where read.
 * \return a connection where read data.
 */
struct o_connection_remote * o_storage_remote_begin_response(struct o_storage_remote * storage);

/*! \brief End a read operation.
 *
 * \param storage where and comunication.
 * @param conn to release.
 */
void o_storage_remote_end_read(struct o_storage_remote * storage, struct o_connection_remote * conn);

/*! \brief Start a conversation write with server for execute an specifed command.
 *
 * \param storage storage to communicate.
 * \param command to exectue
 * \return a new connection to the server.
 */
struct o_connection_remote * o_storage_remote_begin_write(struct o_storage_remote * storage, char command);

/*! \brief End a write conversation with server.
 *
 * \param storage to end conversation.
 * \param conn to release.
 */
void o_storage_remote_end_write(struct o_storage_remote * storage, struct o_connection_remote * conn);

#endif /* O_STORAGE_REMOTE_INTERNAL_H_ */
