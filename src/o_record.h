#ifndef O_RECORD_H_
#define O_RECORD_H_

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

/**
 * Free a record.
 *
 * @param record to free.
 */
void o_record_free(struct o_record * record);

#endif /* O_RECORD_H_ */
