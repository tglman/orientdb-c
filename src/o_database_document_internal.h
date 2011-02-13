#ifndef O_DATABASE_DOCUMENT_INTERNAL_H_
#define O_DATABASE_DOCUMENT_INTERNAL_H_
#include "o_database_document.h"
#include "o_document_formatter.h"

/*! \brief Retrieve the formatter from document database.
 *
 * \param db where retrieve formatter.
 * \return the formatter of database.
 */
struct o_document_formatter * o_database_document_get_formatter(struct o_database_document * db);

#endif /* O_DATABASE_DOCUMENT_INTERNAL_H_ */
