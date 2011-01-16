#ifndef O_LIST_H_
#define O_LIST_H_

struct o_list;

/*! \brief create a new empty list.
 *
 * \return the new list.
 */
struct o_list *o_list_new();

/*! \brief Add an item to the list.
 *
 * \param list where add.
 * \param to_add value to add.
 * \return 1 if the value is added otherwise 0.
 */
int o_list_add(struct o_list * list, void * to_add);

/*! \brief Remove an item from list.
 *
 * \param list where remove.
 * \param to_remove value to remove.
 * \return 1 if the value is removed otherwise 0.
 */
int o_list_remove(struct o_list * list, void *to_remove);

/*! \brief Check if an value is contained in the list.
 *
 * \param list where check.
 * \param to_check value to check.
 * \return 1 if the list contains the value otherwise 0.
 */
int o_list_contains(struct o_list * list, void *to_check);

/*! \brief retrieve the number of values contained in the list.
 *
 * \param list to retrive the size.
 * \return the size.
 */
int o_list_size(struct o_list * list);

/*! \biref retrieve the value at the specified position.
 *
 * \param list where retrive the value.
 * \param pos position of value.
 * \return the value otherwise 0;
 */
void * o_list_get(struct o_list * list, int pos);

/*! \brief free  the memory and structore of list.
 *
 * \param list to free.
 */
void o_list_free(struct o_list * list);

#endif /* O_LIST_H_ */
