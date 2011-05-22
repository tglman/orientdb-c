#ifndef O_STORAGE_REMOTE_INTERNAL_H_
#define O_STORAGE_REMOTE_INTERNAL_H_
#include "o_storage_internal.h"
#include "o_storage_remote.h"

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
	struct o_connection_remote * connection;
	struct o_native_lock * exclusive_lock;
	int session_id;
	int n_cluster;
	struct o_storage_remote_cluster *clusters;
	int default_cluster_id;
};


/*! \brief Manage of start of end of request with error management.
 *
 * \param storage where read, req_id to read.
 * \param req_id to check response.
 */
void o_storage_remote_begin_response(struct o_storage_remote * storage, int req_id);

#endif /* O_STORAGE_REMOTE_INTERNAL_H_ */
