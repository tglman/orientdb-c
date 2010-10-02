#ifndef SOCKET_INTERFACE_H_
#define SOCKET_INTERFACE_H_
#include "o_database_error_handler.h"

struct o_database_socket;

/** connect a new socket.
 *
 * @param site the name of destination site.
 * @param port the destination connection port.
 * @param error_handler the handler of eventually errors.
 */
struct o_database_socket * o_database_socket_connect(char * site, short port, struct o_database_error_handler * error_handler);

/** send a data to a socket.
 *
 * @param sock the destination socket.
 * @param buff the data to send.
 * @param buff_size the length of buffer to write.
 * @param error_handler the handler of eventually errors.
 */
void o_database_socket_send(struct o_database_socket * sock, char * buff, int buff_size, struct o_database_error_handler * error_handler);

/** Receive the data from a socket.
 *
 * @param sock the from socket.
 * @param buff the buffer to fill.
 * @param buff_size the length of buffer to fill.
 * @param error_handler the handler of eventually errors.
 */
void o_database_socket_recv(struct o_database_socket * sock, char * buff, int buff_size, struct o_database_error_handler * error_handler);

/** check if the socket contains data to receive.
 *
 * @param sock to check.
 * @return 0 if the socket not contains data otherwise the number of byte to read.
 * @param error_handler the handler of eventually errors.
 */
int o_database_socket_has_data(struct o_database_socket * sock, struct o_database_error_handler * error_handler);

/** Close the socket e free all structure information.
 *
 * @param sock to free.
 * @param error_handler the handler of eventually errors.
 */
void o_database_socket_close(struct o_database_socket * sock, struct o_database_error_handler * error_handler);

#endif /* SOCKET_INTERFACE_H_ */
