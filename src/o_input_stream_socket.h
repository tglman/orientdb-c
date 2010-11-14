#ifndef O_INPUT_STREAM_SOCKET_C_
#define O_INPUT_STREAM_SOCKET_C_
#include "o_input_stream.h"
#include "o_database_socket.h"

/*! \brief create an input stream on a socket.
 *
 * \param socket where read.
 * \return the new input stream.
 */
struct o_input_stream * o_input_stream_socket_new(struct o_database_socket * socket);

#endif /* O_INPUT_STREAM_SOCKET_C_ */
