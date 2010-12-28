#include "o_storage_internal.h"
#include "o_url_resolver.h"
#include "o_memory.h"
#include <string.h>

long long o_storage_create_record(struct o_storage * storage, int cluster, struct o_raw_buffer * content)
{
	return storage->o_storage_create_record(storage, cluster, content);
}

struct o_raw_buffer * o_storage_read_record(struct o_storage * storage, struct o_record_id * id)
{
	return storage->o_storage_read_record(storage, id);
}

int o_storage_update_record(struct o_storage * storage, struct o_record_id * id, struct o_raw_buffer * content)
{
	return storage->o_storage_update_record(storage, id, content);
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

int o_storage_get_default_cluser_id(struct o_storage * storage)
{
	return storage->o_storage_get_default_cluster_id(storage);
}

void o_storage_commit_transaction(struct o_storage *storage, struct o_transaction * transaction)
{
	return storage->o_storage_commit_transaction(storage, transaction);
}

void o_storage_close(struct o_storage * storage)
{
	return storage->o_storage_close(storage);
}

void o_storage_release(struct o_storage * storage)
{
	storage->ref_count--;
	if (storage->ref_count == 0)
		storage->o_storage_final_release(storage);
}
void o_storage_internal_free(struct o_storage *storage)
{
	o_free(storage->name);
	o_free(storage->user);
}

char * o_storage_get_name(struct o_storage *storage)
{
	return storage->name;
}

char * o_storage_get_user(struct o_storage *storage)
{
	return storage->user;
}

void o_storage_internal_new(struct o_storage *storage, char * name, char * user)
{
	int namelen = strlen(name) + 1;
	storage->name = o_malloc(namelen);
	memcpy(storage->name, name, namelen);
	int usrlen = strlen(user) + 1;
	storage->user = o_malloc(usrlen);
	memcpy(storage->user, user, usrlen);
	storage->ref_count = 0;
}

void o_storage_reference(struct o_storage * storage)
{
	storage->ref_count++;
}

void o_storage_free(struct o_storage * storage)
{
	storage->o_storage_free(storage);
}
