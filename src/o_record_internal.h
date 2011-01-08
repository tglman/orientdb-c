#ifndef O_RECORD_INTERNAL_H_
#define O_RECORD_INTERNAL_H_
#include "o_record.h"
#include "o_output_stream.h"

struct o_record
{
	struct o_record_id * record_id;
	struct o_database * owner;
	char type;
	int version;
	int ref_count;
	char loaded;
	void (*o_record_serialize)(struct o_record * record, struct o_output_stream * stream);
	void (*o_record_deserialize)(struct o_record * record, struct o_input_stream * stream);
	void (*o_record_before_save)(struct o_record * record);
	void (*o_record_after_save)(struct o_record * record);
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

/*! \brief Retrieve the owner of this record.
 *
 * \param record to retrieve the owner.
 * \return the owner of record.
 */
struct o_database * o_record_owner(struct o_record * record);

/*! \brief Check if the record is loaded otherwise load it.
 *
 * \param record to check.
 */
void o_record_check_load(struct o_record * record);

/**
 * Use to actuate free operation on a record.
 *
 * @param record to free.
 */
void o_record_free_internal(struct o_record * record);

#endif /* O_RECORD_INTERNAL_H_ */
