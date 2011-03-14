#ifndef O_OUTPUT_STREAM_SOCKET_H_
#define O_OUTPUT_STREAM_SOCKET_H_
#include "o_output_stream.h"
#include "o_native_socket.h"

/*! \brief Create a new output stream to socket.
 *
 * \param socket where write.
 * \return the new socket output stream.
 */
struct o_output_stream * o_output_stream_socket_new(struct o_native_socket * socket);

#endif /* O_OUTPUT_STREAM_SOCKET_H_ */
