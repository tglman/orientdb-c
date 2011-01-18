#ifndef O_PROPERTY_H_
#define O_PROPERTY_H_
#include "o_document_value.h"

struct o_property;

/*! \brief Retrieve the name of property.
 *
 * \param property the property where retrieve the name.
 * \return the name of property
 */
char * o_property_get_name(struct o_property * property);

/*! \brief Retrieve the type of property.
 *
 * \param property where retrieve the type.
 * \return the type of property.
 */
enum o_document_value_type o_property_get_type(struct o_property * property);

#endif /* O_PROPERTY_H_ */
