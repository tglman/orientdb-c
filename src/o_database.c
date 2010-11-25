#include "o_database.h"
#include "o_database_internal.h"
#include "o_memory.h"
#include <string.h>
#include "o_engine.h"
#include "o_storage.h"
#include "o_url_resolver.h"

struct o_database * o_database_new(char * connection_url)
{
	return o_database_new_error_handler(connection_url, 0);
}

struct o_database * o_database_new_error_handler(char * connection_url, struct o_database_error_handler * error_handler)
{
	struct o_database * new_db = o_malloc(sizeof(struct o_database));
	o_database_new_internal(new_db, connection_url, error_handler);
	return new_db;
}

void o_database_new_internal(struct o_database * db, char * connection_url, struct o_database_error_handler * error_handler)
{
	memset(db, 0, sizeof(struct o_database));
	db->error_handler = error_handler;
	db->connection_url = connection_url;
}

void o_database_reset_error_handler(struct o_database * db, struct o_database_error_handler *error_handler)
{
	db->error_handler = error_handler;
}

void o_database_open(struct o_database * db, char * username, char * password)
{
	db->storage = o_engine_get_storage(db->connection_url, username, password);
}

void o_database_save(struct o_database * db, struct o_record * record)
{
	o_database_save_cluster(db, record, NULL);
}
void o_database_save_cluster(struct o_database * db, struct o_record * record, char * cluster_name)
{
	struct o_record_id *rec_id = o_record_get_id(record);
	int is_new = o_record_id_is_new(rec_id);

	if (is_new)
	{
		int cluster_id = cluster_name != 0 ? o_storage_get_cluster_id_by_name(db->storage, cluster_name) : o_storage_get_default_cluser_id(
				db->storage);
		o_storage_create_record(db->storage, cluster_id, 0);
	}
	else
	{
		o_storage_update_record(db->storage, o_record_get_id(record), 0);
	}
}

void o_database_free_internal(struct o_database * db)
{
	if (db->storage != 0)
		o_storage_free(db->storage);
}

void o_database_free(struct o_database * db)
{
	o_database_free_internal(db);
	o_free(db);
}
