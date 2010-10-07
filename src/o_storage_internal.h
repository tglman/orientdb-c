#ifndef O_STORAGE_INTERNAL_H_
#define O_STORAGE_INTERNAL_H_
#include "o_storage.h"

struct o_storage
{
	char * name;
	long long (*o_storage_create_record)(struct o_storage * storage, int cluster, struct o_record_content * content);
	struct o_record_content * (*o_storage_read_record)(struct o_storage * storage, struct o_record_id * id, int * version);
	int (*o_storage_update_record)(struct o_storage * storage, struct o_record_id * id, int version, struct o_record_content * content);
	int (*o_storage_delete_record)(struct o_storage * storage, struct o_record_id * id, int version);
	char ** (*o_storage_get_cluster_names)(struct o_storage * storage, int * names_count);
	int (*o_storage_get_cluster_id_by_name)(struct o_storage * storage, char * name);
	void (*o_storage_commit_transaction)(struct o_storage *storage, struct o_transaction * transaction);
	void (*o_storage_free)(struct o_storage * storage);
};

/*! \brief Dealloc the storage internal memory.
 *
 *\param storage to free.
 */
void o_storage_internal_free(struct o_storage *storage);

#endif /* O_STORAGE_INTERNAL_H_ */
