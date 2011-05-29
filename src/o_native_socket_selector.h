#ifndef O_NATIVE_SOCKET_SELECTOR_H_
#define O_NATIVE_SOCKET_SELECTOR_H_
#include "o_native_socket.h"


enum selector_mode
{
	READ,WRITE,ERROR
};

struct o_native_socket_selector;

/*! \brief Create a new socket selector.
 *
 * \param mode the mode of select
 * \return the new selector.
 */
struct o_native_socket_selector * o_native_socket_selector_new(enum selector_mode mode);

/*! \brief add a socket to the selector.
 *
 * \param selector where add a socket.
 * \param socket to add.
 */
void o_native_socket_selector_add_socket(struct o_native_socket_selector * selector, struct o_native_socket* socket);

/*! \brief Remove a socket from the selector.
 *
 * \param selector where remove.
 * \param socket to remove.
 */
void o_native_socket_selector_remove_socket(struct o_native_socket_selector * selector, struct o_native_socket* socket);

/*! \brief Start the select of socket.
 *
 * \param selector to select.
 * \param select timeout.
 * \return an selected socket or 0 if select fail.
 */
struct o_native_socket* o_native_socket_selector_select(struct o_native_socket_selector * selector, int timeout);

/*!\brief free the selector.
 *
 * \param selector to free.
 */
void o_native_socket_selector_free(struct o_native_socket_selector * selector);

#endif /* O_NATIVE_SOCKET_SELECTOR_H_ */
