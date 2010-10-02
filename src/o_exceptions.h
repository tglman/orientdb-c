#ifndef O_EXCEPTIONS_H_
#define O_EXCEPTIONS_H_
#include <setjmp.h>
#include "o_exception.h"

/*! \brief check if the exception is of type specified.
 *
 * \param name of the type of exception.
 * \return 1 if the type specified by name match otherwise 0.
 */
int o_catch_type(char * name);

/*! \brief Retrieve the poiter of exception.
 *
 * \return the value of current exception.
 */
void * o_catch_value();

/*! \brief manage a throw of exception.
 *
 * \param name of type of exception.
 * \param value the instance of exception.
 */
void o_notify_object(char * name, void * value);

/*! \brief throw an exception.
 *
 * \param exception to throw.
 */
void o_notify_exception(struct o_exception * exception);

/*! \brief the base jmp variable.
 */
extern __thread jmp_buf exception_try_pos_jmp;

/*! \brief begin block try.
 */
#define try if(setjmp(exception_try_pos_jmp)==0 && push(exception_try_pos_jmp))

/*! \brief begin block catch.
 */
#define catch(VAR)  else if(o_catch_type(#VAR))

/*! \brief retrieve the current exception var.
 *
 */
#define exception(VAR) (VAR *)o_catch_value()

/*! \brief throw a new exception.
 *
 */
#define throw_type(TYPE,INSTANCE) o_notify_exception(#TYPE,(TYPE *)INSTANCE)

#define throw(EXCEPTION) o_notify_exception((struct o_exception *)EXCEPTION)

#endif /* O_EXCEPTIONS_H_ */
