#ifndef O_RECORD_H_
#define O_RECORD_H_
#include "o_record_id.h"
#include "o_row_buffer.h"
#include "o_output_stream.h"

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

/*! \brief Retrieve the type of record.
 *
 * \return the byte that describe the type of record.
 */
char o_record_type(struct o_record * record);

/*! /brief retrieve the content of record.
 *
 * \param record from read content.
 * \return the buffer content.
 */
struct o_raw_buffer * o_record_content(struct o_record * record);

/*! \brief Serialize the record to output stream.
 *
 * \param record to write.
 * \param output where write;
 */
void o_record_serialize(struct o_record * record, struct o_output_stream * output);

/**
 * Free a record.
 *
 * @param record to free.
 */
void o_record_free(struct o_record * record);

#endif /* O_RECORD_H_ */
