#include "o_storage_internal.h"
#include "o_url_resolver.h"


long long o_storage_create_record(struct o_storage * storage, int cluster, struct o_record_content * content)
{
	return storage->o_storage_create_record(storage, cluster, content);
}

struct o_record_content * o_storage_read_record(struct o_storage * storage, struct o_record_id * id, int * version)
{
	return storage->o_storage_read_record(storage, id, version);
}

int o_storage_update_record(struct o_storage * storage, struct o_record_id * id, int version, struct o_record_content * content)
{
	return storage->o_storage_update_record(storage, id, version, content);
}

int o_storage_delete_record(struct o_storage * storage, struct o_record_id * id, int version)
{
	return storage->o_storage_delete_record(storage, id, version);
}

char ** o_storage_get_cluster_names(struct o_storage * storage, int * names_count)
{
	return storage->o_storage_get_cluster_names(storage, names_count);
}

int o_storage_get_cluster_id_by_name(struct o_storage * storage, char * name)
{
	return storage->o_storage_get_cluster_id_by_name(storage, name);
}

void o_storage_commit_transaction(struct o_storage *storage, struct o_transaction * transaction)
{
	return storage->o_storage_commit_transaction(storage, transaction);
}

void o_storage_free(struct o_storage * storage)
{
	storage->o_storage_free(storage);
}
