#ifndef O_PROPERTY_INTERNAL_H_
#define O_PROPERTY_INTERNAL_H_
#include "o_property.h"

/*! \brief Create a new property with name and type.
 *
 * \param name the name of property.
 * \param type the type of property.
 * \return the new property.
 */
struct o_property * o_property_new(char * name, enum o_document_value_type type);

/*! \brief Create a new property from a document.
 *
 * \param document to deserialize for generate property.
 * \return a new instance of property.
 */
struct o_property * o_property_new_from_document(struct o_document *document);

/*! \brief Free property memory.
 *
 * \param property to free
 */
void o_property_free(struct o_property * property);

#endif /* O_PROPERTY_INTERNAL_H_ */
