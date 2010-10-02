#ifndef O_EXCEPTION_H_
#define O_EXCEPTION_H_

/*! \brief describe a base exception.
 *
 */
struct o_exception;

/*! \brief Create a ne exception.
 *
 * \param message of exception.
 * \param code of exception.
 * \return the new exception instance.
 */
struct o_exception * o_exception_new(char * message, int code);

/*! \brief Retrieve the message of exception.
 *
 * \param exception where retrieve message.
 * \return the value of message of exception.
 */
char * o_exception_message(struct o_exception *exception);

/*! \brief Retrieve the error code of exception.
 *
 * \param exception where retrieve error code.
 * \return the error code of exception.
 */
int o_exception_code(struct o_exception *exception);

/*! \brief Retrieve the string with information of exception.
 *
 * \param exception where retrieve exception.
 * \return the string that describe the exception.
 */
char * o_exception_what(struct o_exception * exception);

/*! \brief check if type specified bype type has match by exception.
 *
 * \param exception where verify.
 * \param type of destinatio to verify.
 * \return 1 if the type match otherwise 0.
 */
int o_exception_typeof(struct o_exception * exception, char * type);

/*! \brief free an exception.
 *
 * \param exception to free.
 */
void o_exception_free(struct o_exception * exception);

#endif /* O_EXCEPTION_H_ */
