#ifndef O_DATABASE_ERROR_HANDLER_H_
#define O_DATABASE_ERROR_HANDLER_H_

/** Represent a structure for handle the database errors.
 *
 */
struct o_database_error_handler;

/** The Type of function for handling error.
 *
 * @param code the error code.
 * @param error the description of error.
 * @param custom_info the user defined additional information.
 */
typedef void (*o_database_error_handler_function)(int code, char * error, void * custom_info);

/** The Error handler create.
 *
 * @param function that handler error.
 * @param custom_info user defined information.
 * @return a new error hanlder.
 */
struct o_database_error_handler * o_database_error_handler_new(o_database_error_handler_function function, void * custom_info);

/*! \biref Create the default error handler.
 *
 * \return the new error handler instance.
 */
struct o_database_error_handler * o_database_error_handler_new_default();

/** Notify an error to the handler.
 *
 * @param error_handler target of notification.
 * @param code the code of error.
 * @param error the description of the error.
 */
void o_database_error_handler_notify(struct o_database_error_handler * error_handler, int code, char * error);

/** Free an error handler information.
 *
 * @param error_handler to free.|
 */
void o_database_error_handler_free(struct o_database_error_handler * error_handler);

#endif /* O_DATABASE_ERROR_HANDLER_H_ */
