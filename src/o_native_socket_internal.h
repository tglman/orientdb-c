#ifndef O_NATIVE_SOCKET_INTERNAL_H_
#define O_NATIVE_SOCKET_INTERNAL_H_
#include "o_native_socket.h"


struct o_native_socket
{
	int socket;
};

/*! \brief Retrieve the internal descriptor for the socket.
 *
 * \param sock to retrieve the internal descriptor.
 * \return the descriptor.
 */
int o_native_socket_internal_descriptor(struct o_native_socket * sock);

/*! \brief connect a new socket using existing data structure.
 *
 * \param conn connection
 * \param site the name of destination site.
 * \param port the destination connection port.
 */
void o_native_socket_connect_internal(struct o_native_socket * sock,char * site, short port);

/*! \brief Create a new socket from an listen socket for accept.
 * 			this function return only after accept.
 *
 * \param listen the socket in listening.
 * \param to_fill socket to fill.
 * \return a new socket from accepted connection.
 */
void o_native_socket_accept_internal(struct o_native_socket * listen,struct o_native_socket * to_fill);

/** Close the socket e .
 *
 * \param sock to close.
 */
void o_native_socket_close_internal(struct o_native_socket * sock);


#endif /* O_NATIVE_SOCKET_INTERNAL_H_ */
