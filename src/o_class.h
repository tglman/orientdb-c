#ifndef O_CLASS_H_
#define O_CLASS_H_
#include "o_property.h"

struct o_class;

/*! \brief Retrieve the name of class from class
 *
 * \param class where retrieve name.
 * \return the name of class.
 */
char * o_class_get_name(struct o_class * class);

/*! \brief Retrieve the superclass of class.
 *
 * \param class where retrieve the superclass.
 * \return the superclass or 0 if not exist superclass.
 */
struct o_class * o_class_get_superclass(struct o_class * class);

/*! \brief Set the superclass of class.
 *
 * \param class to set superclass.
 * \param superclass to set.
 */
void o_class_set_superclass(struct o_class * class, struct o_class * superclass);

/*! \brief Retrieve a property from class.
 *
 * \param class where retrieve property.
 * \param name of property to retrieve.
 * \return the property or 0 if the property not exist.
 */
struct o_property * o_class_get_property(struct o_class * class, char * name);

/*! \brief Create a new property on class with specialized type.
 *
 * \param class where create.
 * \param name of property to create.
 * \param type of property to create.
 * \return the new property.
 */
struct o_property * o_class_create_property(struct o_class * class, char * name, enum o_document_value_type type);

#endif /* O_CLASS_H_ */
