#ifndef O_NATIVE_SOCKET_INTERNAL_H_
#define O_NATIVE_SOCKET_INTERNAL_H_
#include "o_native_socket.h"

/*! \brief Retrieve the internal descriptor for the socket.
 *
 * \param sock to retrieve the internal descriptor.
 * \return the descriptor.
 */
int o_native_socket_internal_descriptor(struct o_native_socket * sock);

#endif /* O_NATIVE_SOCKET_INTERNAL_H_ */
