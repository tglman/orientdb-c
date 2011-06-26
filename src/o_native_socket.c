#include "o_native_socket.h"
#include "o_native_socket_internal.h"
#include "o_exceptions.h"
#include "o_exception_io.h"
#include "o_memory.h"
#include <malloc.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>

void o_native_socket_connect_internal(struct o_native_socket * sock, char * site, short port)
{
	struct sockaddr_in sock_info;
	//TODO: Retrieve the socket information by the name of site.
	struct hostent *he = gethostbyname(site);
	sock->socket = socket(AF_INET, SOCK_STREAM, 0);
	sock_info.sin_family = AF_INET;
	sock_info.sin_port = htons(port);
	memcpy(&sock_info.sin_addr, he->h_addr, he->h_length);
	int res = connect(sock->socket, (struct sockaddr*) &sock_info, sizeof(sock_info));
	if (res == -1)
	{
		char err_message[512];
		sprintf(err_message, "Error on connection:%s ", strerror(errno));
		throw(o_exception_io_new(err_message,3));
	}
}
struct o_native_socket * o_native_socket_connect(char * site, short port)
{
	struct o_native_socket *sock = 0;
	try
	{
		sock = o_malloc(sizeof(struct o_native_socket));
		o_native_socket_connect_internal(sock, site, port);
	}
	catch(struct o_exception, ex)
	{
		o_free(sock);
		throw(ex);
	}
	end_try;
	return sock;
}

struct o_native_socket * o_native_socket_listen(char * site, short port)
{
	struct o_native_socket *sock = 0;
	try
	{
		sock = o_malloc(sizeof(struct o_native_socket));
		memset(sock, 0, sizeof(struct o_native_socket));
		int res;
		sock->socket = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in listen_conf;
		listen_conf.sin_family = AF_INET;
		struct hostent *he = gethostbyname(site);
		memcpy(&listen_conf.sin_addr, he->h_addr, he->h_length);
		listen_conf.sin_port = htons(port);
		res = bind(sock->socket, (struct sockaddr*) &listen_conf, sizeof(listen_conf));
		if (res == -1)
		{
			char err_message[512];
			sprintf(err_message, "Error on bind socket:%s ", strerror(errno));
			throw(o_exception_io_new(err_message,3));
		}

		res = listen(sock->socket, 7);
		if (res == -1)
		{
			char err_message[512];
			sprintf(err_message, "Error on start listen socket:%s ", strerror(errno));
			throw(o_exception_io_new(err_message,3));
		}
	}
	catch(struct o_exception, ex)
	{
		o_free(sock);
		throw(ex);
	}
	end_try;
	return sock;
}

void o_native_socket_accept_internal(struct o_native_socket * listen, struct o_native_socket * to_fill)
{
	struct sockaddr_in accept_sock;
	unsigned int len = sizeof(struct sockaddr_in);
	to_fill->socket = accept(listen->socket, (struct sockaddr*) &accept_sock, &len);
	if (to_fill->socket == -1)
		throw(o_exception_io_new("error on accept socket",3));
}

struct o_native_socket * o_native_socket_accept(struct o_native_socket * listen_sock)
{
	struct o_native_socket *sock = 0;
	try
	{
		sock = o_malloc(sizeof(struct o_native_socket));
		o_native_socket_accept_internal(listen_sock, sock);
	}
	catch(struct o_exception, ex)
	{
		o_free(sock);
		throw(ex);
	}
	end_try;
	return sock;

}

void o_native_socket_send(struct o_native_socket * sock, void * buff, int buff_size)
{
	int already_send = 0;
	int cur_ret = 0;
	while (already_send < buff_size && cur_ret != -1)
	{
		cur_ret = send(sock->socket, buff + already_send, buff_size - already_send, 0);
		if (cur_ret != -1)
			already_send += cur_ret;
	}
}

void o_native_socket_recv(struct o_native_socket * sock, void * buff, int *buff_size, int params)
{
	int sock_par = 0;
	int already_receved = 0;
	int cur_ret = 0;
	if (params == READ_PEEK)
		sock_par = MSG_PEEK;
	while (already_receved < *buff_size && cur_ret != -1)
	{
		cur_ret = recv(sock->socket, buff + already_receved, (*buff_size) - already_receved, sock_par);
		if (cur_ret != -1)
			already_receved += cur_ret;
	}
	*buff_size = already_receved;
}

int o_native_socket_has_data(struct o_native_socket * sock)
{
	int size = 0;
	ioctl(sock->socket, FIONREAD, &size);
	return size;
}

void o_native_socket_close_internal(struct o_native_socket * sock)
{
	shutdown(sock->socket, SHUT_RDWR);
	close(sock->socket);
}

int o_native_socket_internal_descriptor(struct o_native_socket * sock)
{
	return sock->socket;
}

void o_native_socket_close(struct o_native_socket * sock)
{
	o_native_socket_close_internal(sock);
	o_free(sock);
}
