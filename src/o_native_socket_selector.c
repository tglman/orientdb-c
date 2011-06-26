#include "o_native_socket_selector.h"
#include "o_memory.h"
#include "o_list.h"
#include "o_native_socket_internal.h"
#include "o_native_lock.h"
#include "o_exceptions.h"
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define max(x,y) ((x) > (y) ? (x) : (y))

struct o_native_socket_selector
{
	struct o_native_lock * socks_lock;
	int epoll;
};

struct o_native_socket_selector * o_native_socket_selector_new()
{
	struct o_native_socket_selector * selector = o_malloc(sizeof(struct o_native_socket_selector));
	selector->socks_lock = o_native_lock_new();
	selector->epoll = epoll_create(10);
	return selector;
}

void o_native_socket_selector_add_socket(struct o_native_socket_selector * selector, struct o_native_socket* sock)
{
	o_native_lock_lock(selector->socks_lock);
	struct epoll_event ev;
	memset(&ev, 0, sizeof(struct epoll_event));
	int sock_dec = o_native_socket_internal_descriptor(sock);
	ev.events = EPOLLIN | EPOLLONESHOT;
	ev.data.ptr = sock;
	epoll_ctl(selector->epoll, EPOLL_CTL_ADD, sock_dec, &ev);
	o_native_lock_unlock(selector->socks_lock);
}

void o_native_socket_selector_remove_socket(struct o_native_socket_selector * selector, struct o_native_socket* sock)
{
	o_native_lock_lock(selector->socks_lock);
	struct epoll_event ev;
	memset(&ev, 0, sizeof(struct epoll_event));
	int sock_dec = o_native_socket_internal_descriptor(sock);
	epoll_ctl(selector->epoll, EPOLL_CTL_DEL, sock_dec, &ev);
	o_native_lock_unlock(selector->socks_lock);
}

struct o_native_socket* o_native_socket_selector_select(struct o_native_socket_selector * selector, int timeout)
{
	struct epoll_event events[1];
	o_native_lock_lock(selector->socks_lock);
	while (1)
	{
		int select_ret = epoll_wait(selector->epoll, events, 1, timeout);
		if (select_ret == 1)
		{
			o_native_lock_unlock(selector->socks_lock);
			return (struct o_native_socket*) events[0].data.ptr;
		}
		else if (select_ret == 0)
		{
			o_native_lock_unlock(selector->socks_lock);
			return 0;
		}
		else if (select_ret == -1)
		{
			if (errno == EINTR)
				continue;
			else
				throw(o_exception_new("Error on socket selector", errno));
		}
	}
	o_native_lock_unlock(selector->socks_lock);
	return 0;
}

void o_native_socket_selector_end_select(struct o_native_socket_selector * selector, struct o_native_socket* sock)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(struct epoll_event));
	int sock_dec = o_native_socket_internal_descriptor(sock);
	ev.events = EPOLLIN | EPOLLONESHOT;
	ev.data.ptr = sock;
	epoll_ctl(selector->epoll, EPOLL_CTL_MOD, sock_dec, &ev);

}

void o_native_socket_selector_free(struct o_native_socket_selector * selector)
{
	o_native_lock_free(selector->socks_lock);
	o_free(selector);
}
