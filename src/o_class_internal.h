#ifndef O_CLASS_INTERNAL_H_
#define O_CLASS_INTERNAL_H_
#include "o_class.h"

/*! \brief create a new class with name.
 *
 * \param name of class
 * \return a new class instance.
 */
struct o_class * o_class_new(char * name);

/*! \brief Free the memory and substructure of class.
 *
 * \param class to free.
 */
void o_class_free(struct o_class * class);

#endif /* O_CLASS_INTERNAL_H_ */
