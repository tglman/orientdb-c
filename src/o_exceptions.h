#ifndef O_EXCEPTIONS_H_
#define O_EXCEPTIONS_H_
#include <setjmp.h>
#include "o_exception.h"

/*! \brief check if the exception is of type specified.
 *
 * \param name of the type of exception.
 * \return 1 if the type specified by name match otherwise 0.
 */
int o_catch_type(char * name,void **val,jmp_buf cur_jmp);

/*! \brief push a begin of a try.
 *
 * \param cur_jmp to push in stack
 */
int o_try_push(jmp_buf cur_jmp);

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

/*! \brief begin block try.
 */
#define try jmp_buf current_try_pos_jmp;if(setjmp(current_try_pos_jmp)==0 && o_try_push(current_try_pos_jmp))

/*! \brief begin block catch.
 */
#define catch(TYPE,VAR_NAME) TYPE *VAR_NAME; if(o_catch_type(#TYPE,(void **)&VAR_NAME,current_try_pos_jmp))

/*! \brief throw a new exception.
 *
 */
#define throw_type(TYPE,INSTANCE) o_notify_object(#TYPE,(TYPE *)INSTANCE)

#define throw(EXCEPTION) o_notify_exception((struct o_exception *)EXCEPTION)

#endif /* O_EXCEPTIONS_H_ */
