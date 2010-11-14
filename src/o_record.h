#ifndef O_RECORD_H_
#define O_RECORD_H_

#include "o_record_id.h"

/**
 * The record.
 */
struct o_record;

/**
 * create a new record.
 *
 * @return the new record.
 */
struct o_record * o_record_new();

/*! \brief Retrieve the record id of record.
 *
 * \param record where retrive id.
 * \return the record id.
 */
struct o_record_id * o_record_get_id(struct o_record * record);

/*! \brief Retrieve the version of current record.
 *
 * \param the record where retrieve the version.
 * \return the version of record.
 */
int o_record_version(struct o_record * record);

/**
 * Free a record.
 *
 * @param record to free.
 */
void o_record_free(struct o_record * record);

#endif /* O_RECORD_H_ */
