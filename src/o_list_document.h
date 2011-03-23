#ifndef O_LIST_DOCUMENT_H_
#define O_LIST_DOCUMENT_H_
#include "o_document.h"

struct o_list_document;

/*! \brief Create a new document list.
 *
 * \return a new document list.
 */
struct o_list_document * o_list_document_new();

/*! \brief add a document to the list.
 *
 * \param list where add.
 * \param to_add document to add.
 */
void o_list_document_add(struct o_list_document * list, struct o_document * to_add);

/*! \brief Retrieve a document from the list at the position.
 *
 * \param list where get.
 * \param pos position of document to get.
 * \return the record instance or 0 if not exist an record in the position.
 */
struct o_document * o_list_document_get(struct o_list_document * list, int pos);

/*! \brief Retrieve the size of the list.
 *
 * \param list where retrieve the size.
 * \return the size of list.
 */
int o_list_document_size(struct o_list_document * list);

/*! \brief Free the list structure and data.
 *
 * \param list to free.
 */
void o_list_document_free(struct o_list_document * list);

#endif /* O_LIST_DOCUMENT_H_ */
