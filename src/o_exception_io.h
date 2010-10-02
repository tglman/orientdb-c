#ifndef O_EXCEPTION_IO_H_
#define O_EXCEPTION_IO_H_

struct o_exception_io;

/*! \brief Create a new io exception.
 *
 * \param message of exception.
 * \parma code of exception.
 */
struct o_exception_io *o_exception_io_new(char * message, int code);

#endif /* O_EXCEPTION_IO_H_ */
