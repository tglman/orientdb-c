#ifndef O_RECORD_INTERNAL_H_
#define O_RECORD_INTERNAL_H_
#include "o_record.h"
#include "o_output_stream.h"

struct o_record
{
	struct o_record_id * record_id;
	char type;
	int version;
	void (*o_record_serialize)(struct o_record * record, struct o_output_stream * stream);
};

/**
 * Use to actuate constructor operation on a record.
 *
 * @param record to build.
 */
void o_record_new_internal(struct o_record * record, char type);

/**
 * Use to actuate free operation on a record.
 *
 * @param record to free.
 */
void o_record_free_internal(struct o_record * record);

#endif /* O_RECORD_INTERNAL_H_ */
