#ifndef O_SCHEMA_H_
#define O_SCHEMA_H_
#include "o_class.h"

struct o_schema;

/*! \brief Retreive the class from the schema.
 *
 * \param schema where search.
 * \param name of class to search.
 * \return the class for the name otherwise 0 if class with the name not exist.
 */
struct o_class * o_schema_get_class(struct o_schema * schema, char * name);

/*! \brief Create a class on the schema with name.
 *
 * \param schema where create.
 * \param name of class to create.
 * \return the new class.
 */
struct o_class * o_schema_create_class(struct o_schema * schema, char * name);

/*! \brief Retrieve the list of classes from database.
 *
 * \param schema where retrive the classes.
 * \param n_classes filled with the number of classes.
 * \return an array of classes.
 */
struct o_class ** o_schema_get_classes(struct o_schema * schema, int * n_classes);

#endif /* O_SCHEMA_H_ */
