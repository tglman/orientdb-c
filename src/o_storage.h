#ifndef O_STORAGE_H_
#define O_STORAGE_H_
#include "o_record_content.h"
#include "o_database_error_handler.h"
#include "o_record_id.h"
#include "o_transaction.h"

struct o_storage;

/**
 * Create a new record to the storage.
 *
 * @param storage where create the record.
 * @param cluster where put the record.
 * @param the record content.
 * @return the new identifier of created record.
 */
long long o_storage_create_record(struct o_storage * storage, int cluster, struct o_record_content * content);

/**
 * Read a record from the storage
 *
 * @param storage the storage where read.
 * @param id the identifier of record to read.
 * @param version poiter to int filled with te version fo current readed record.
 * @return the content of read record
 *
 */
struct o_record_content * o_storage_read_record(struct o_storage * storage, struct o_record_id * id, int * version);

/**
 * Update a record to the storage.
 *
 * @param storage where create the record.
 * @param id of record to update.
 * @param version of record to update.
 * @param content the record content.
 * @return the new number of version of updated record.
 */
int o_storage_update_record(struct o_storage * storage, struct o_record_id * id, int version, struct o_record_content * content);

/**
 * Delete a record from a storage.
 *
 * @param storage the storage where delete the record.
 * @param id the identifier of record to delete.
 * @param vesion the version of record to delete.
 */
int o_storage_delete_record(struct o_storage * storage, struct o_record_id * id, int version);

/**
 * Retrieve the names of clusters in storage.
 *
 * @param storage storage where retrieve the names.
 * @param names_count the count of names retrieved names.
 * @return the names of clusters in storage.
 */
char ** o_storage_get_cluster_names(struct o_storage * storage, int * names_count);

/**
 * Retrieve the id of cluster from the name.
 * @param the storage where search the cluster.
 * @param name the name of cluster.
 * @return the identifier of cluster.
 */
int o_storage_get_cluster_id_by_name(struct o_storage * storage, char * name);

/**
 * Commit a transaction to the storage.
 *
 * @param storage where commit the transuction.
 * @param transaction to commit.
 */
void o_storage_commit_transaction(struct o_storage *storage, struct o_transaction * transaction);

/** Free the storage structure.
 *
 * @param storage to free.
 */
void o_storage_free(struct o_storage * storage);

#endif /* O_STORAGE_H_ */
