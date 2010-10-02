#ifndef O_RECORD_ID_H_
#define O_RECORD_ID_H_

struct o_record_id;

/** Create a new record id with empty values.
 *
 * @return a new instance of record id.
 */
struct o_record_id * o_record_id_new_empty();

/** Create a new record id.
 *
 * @param cluster_id the identifier of cluster.
 * @param record_id the identifier of record.
 * @return a new instance of record id.
 */
struct o_record_id * o_record_id_new(int cluster_id, long long record_id);

/** Retrieve the identifier of cluster from a record id.
 *
 * @param o_id the record identifier where retrieve the cluster identifier.
 * @return the cluster identifier.
 */
int o_record_id_cluster_id(struct o_record_id * o_id);

/** Retrieve the identifier of record from a record id.
 *
 * @param o_id the record identifier where retrieve the in cluster record identifier.
 * @return the in cluster record identifier.
 */
long long o_record_id_record_id(struct o_record_id * o_id);

/** Free an record id.
 *
 * @param o_id the record id to free.
 */
void o_record_id_free(struct o_record_id * o_id);

#endif /* O_RECORD_ID_H_ */
