#ifndef O_RECORD_INTERNAL_H_
#define O_RECORD_INTERNAL_H_
#include "o_record.h"
#include "o_raw_buffer.h"
#include "o_output_stream.h"

struct o_record
{
	struct o_record_id * record_id;
	char type;
	int version;
	int ref_count;
	char loaded;
	void (*o_record_serialize)(struct o_record * record, struct o_output_stream * stream);
	void (*o_record_deserialize)(struct o_record * record, struct o_input_stream * stream);
	void (*o_record_before_save)(struct o_record * record);
	void (*o_record_after_save)(struct o_record * record);
	char *(*o_record_cluster_name)(struct o_record * record);
	void (*o_record_free)(struct o_record * record);
};

/**
 * Use to actuate constructor operation on a record.
 *
 * @param record to build.
 */
void o_record_new_internal(struct o_record * record, char type);

/*! \brief Create a new record with id.
 *
 * \param record.
 * \param type of record.
 * \param rid the current record id.
 */
void o_record_new_internal_id(struct o_record * record, char type, struct o_record_id *rid);

/*! \brief Reset the record with a new version.
 *
 * \param record to reset.
 * \param new_version to set.
 */
void o_record_reset_version(struct o_record * record, int new_version);

/*! \brief invoke listener on record before save action.
 *
 * \param record saved.
 */
void o_record_before_save(struct o_record * record);

/*! \brief Reset the identifier of record
 *
 * \param record where reset identifier.
 * \param new_id to set.
 */
void o_record_reset_id(struct o_record * record, struct o_record_id *new_id);

/*! \brief invoke listener on record after save action.
 *
 * \param record saved.
 */
void o_record_after_save(struct o_record * record);

/*! \brief Retrieve the cluster where the record must be saved.
 *
 * \param record to save.
 * \return the name of cluster where save the record.
 */
char * o_record_cluster_name(struct o_record * record);

/*! /brief retrieve the content of record.
 *
 * \param record from read content.
 * \return the buffer content.
 */
struct o_raw_buffer * o_record_content(struct o_record * record);

/**
 * Use to actuate free operation on a record.
 *
 * @param record to free.
 */
void o_record_free_internal(struct o_record * record);

#endif /* O_RECORD_INTERNAL_H_ */
