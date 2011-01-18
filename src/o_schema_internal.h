#ifndef O_SCHEMA_INTERNAL_H_
#define O_SCHEMA_INTERNAL_H_
#include "o_schema.h"
#include "o_database.h"

/*! \brief Load a schema from database.
 *
 * \param db where load schema.
 * \return the new loaded schema.
 */
struct o_schema * o_schema_load(struct o_database * db);

/*! \brief Save a schema to database.
 *
 * \param schema to save.
 * \param db to save.
 */
void o_schema_save(struct o_schema * schema, struct o_database * db);

#endif /* O_SCHEMA_INTERNAL_H_ */
