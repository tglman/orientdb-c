#ifndef O_STORAGE_EXCEPTION_H_
#define O_STORAGE_EXCEPTION_H_
#include "o_exceptions.h"

struct o_storage_exception;

/*! \brief Create a new Storage exception.
 *
 * \param message the message of exception.
 * \param code the error code of exception.
 * \return the new exception.
 */
struct o_storage_exception * o_storage_exception_new(char *message,int code);

#endif /* O_STORAGE_EXCEPTION_H_ */
