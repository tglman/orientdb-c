#ifndef O_RECORD_INTERNAL_H_
#define O_RECORD_INTERNAL_H_
#include "o_record.h"
#include "o_record_id.h"

struct o_record
{
	struct o_record_id * record_id;
	int version;
};

/**
 * Use to actuate constructor operation on a record.
 *
 * @param record to build.
 */
void o_record_new_internal(struct o_record * record);

/**
 * Use to actuate free operation on a record.
 *
 * @param record to free.
 */
void o_record_free_internal(struct o_record * record);

#endif /* O_RECORD_INTERNAL_H_ */
