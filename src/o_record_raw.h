#ifndef O_RECORD_RAW_H_
#define O_RECORD_RAW_H_
#define RAW_RECORD_TYPE 'b'
#include "o_record.h"

/*! \brief Create a new empty byte.
 *
 * \return a new record.
 */
struct o_record * o_record_raw_new();

/*! \brief Create a new record.
 *
 * \param content the content to put in  new record.
 * \param content_len the length of content to put.
 * \return a new record.
 */
struct o_record * o_record_raw_new_content(char * content, int content_len);

/*! \brief create a new raw record with id.
 *
 * \param id of raw record.
 */
struct o_record * o_record_raw_new_id(struct o_record_id * rid);

/*! \brief fill the raw record with the content.
 *
 * \param record to fill.
 * \param content the content to put in record.
 * \param content_len the length of content to put.
 *
 */
void o_record_raw_reset(struct o_record * record, char * content, int content_len);

/*! \brief retrieve the content from a raw record.
 *
 * \param record where retrieve data.
 * \param content_len filled with the len of content.
 * \return the content of
 */
char * o_record_raw_content(struct o_record * record, int *content_len);

#endif /* O_RECORD_RAW_H_ */
