#ifndef O_RECORD_LIST_H_
#define O_RECORD_LIST_H_
#include "o_record.h"

struct o_list_record;

/*! \brief Create a new list of record.
 *
 * \return the new record list.
 */
struct o_list_record * o_list_record_new();

/*! \brief Retrieve the size of list of record.
 *
 * \param list to retrieve the size.
 * \return the size of record.
 */
int o_list_record_size(struct o_list_record * list);

/*! \brief Retrieve a record at the position.
 *
 * \param list where retrieve the record.
 * \param pos position fo record to retrieve.
 */
struct o_record * o_list_record_get(struct o_list_record * list, int pos);

/*! \brief Add an record to the back of list.
 *
 * \param list where add.
 * \param record to add.
 */
void o_list_record_add(struct o_list_record * list, struct o_record * record);

/*! \brief Release the reference to record list.
 *
 * \param list to release the reference.
 */
void o_list_record_release(struct o_list_record * list);

#endif /* O_RECORD_LIST_H_ */
