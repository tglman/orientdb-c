#ifndef O_SCHEMA_INTERNAL_H_
#define O_SCHEMA_INTERNAL_H_
#include "o_schema.h"
#include "o_database.h"
#include "o_document.h"

/*! \brief Retrieve the schema from a document.
 *
 * \param document where read the schema.
 * \return a new schema from document.
 */
struct o_schema * o_schema_from_document(struct o_document * doc);

/*! \brief Free the momory of schema with all dependent structure.
 *
 * \param schema to free.
 */
void o_schema_free(struct o_schema * schema);

#endif /* O_SCHEMA_INTERNAL_H_ */
