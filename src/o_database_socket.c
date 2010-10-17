#include "o_database_socket.h"
#include "o_exceptions.h"
#include "o_exception_io.h"
#include "o_memory.h"
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

struct o_database_socket
{
	int socket;
};

struct o_database_socket * o_database_socket_connect(char * site, short port)
{
	struct o_database_socket *sock = 0;
	try
	{
		struct sockaddr_in sock_info;
		//TODO: Retrieve the socket information by the name of site.
		struct hostent *he = gethostbyname(site);
		sock = o_malloc(sizeof(struct o_database_socket));
		sock->socket = socket(AF_INET, SOCK_STREAM, 0);
		sock_info.sin_family = AF_INET;
		sock_info.sin_port = htons(port);
		memcpy(&sock_info.sin_addr, he->h_addr, he->h_length);
		int res = connect(sock->socket, (struct sockaddr*) &sock_info, sizeof(sock_info));
		if (res == -1)
			throw(o_exception_io_new("error on open connection",3));
	}
	catch(struct o_exception, ex)
	{
		o_free(sock);
		throw(ex);
	}
	return sock;
}

struct o_database_socket * o_database_socket_listen(char * site, short port)
{
	struct o_database_socket *sock = 0;
	try
	{
		sock = o_malloc(sizeof(struct o_database_socket));
		int res;
		sock->socket = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in listen_conf;
		listen_conf.sin_family = AF_INET;
		struct hostent *he = gethostbyname(site);
		//inet_pton(AF_INET, site, &listenConf.sin_addr);
		memcpy(&listen_conf.sin_addr, he->h_addr, he->h_length);
		listen_conf.sin_port = htons(port);
		res = bind(sock->socket, (struct sockaddr*) &listen_conf, sizeof(listen_conf));
		if (res == -1)
			throw(o_exception_io_new("error on bind socket",3));
		res = listen(sock->socket, 7);
		if (res == -1)
			throw(o_exception_io_new("error on start listen socket",3));
	}
	catch(struct o_exception, ex)
	{
		o_free(sock);
		throw(ex);
	}
	return sock;
}

struct o_database_socket * o_database_socket_accept(struct o_database_socket * listen)
{
	struct o_database_socket *sock = 0;
	try
	{
		sock = o_malloc(sizeof(struct o_database_socket));
		struct sockaddr_in accept_sock;
		unsigned int len = sizeof(struct sockaddr_in);
		sock->socket = accept(listen->socket, (struct sockaddr*) &accept_sock, &len);
		if (sock->socket == -1)
			throw(o_exception_io_new("error on accept socket",3));
	}
	catch(struct o_exception, ex)
	{
		o_free(sock);
		throw(ex);
	}
	return sock;

}

void o_database_socket_send(struct o_database_socket * sock, void * buff, int buff_size)
{
	send(sock->socket, buff, buff_size, 0);
}

void o_database_socket_recv(struct o_database_socket * sock, void * buff, int *buff_size)
{
	*buff_size = recv(sock->socket, buff, *buff_size, 0);
}

int o_database_socket_has_data(struct o_database_socket * sock)
{
	int size = 0;
	ioctl(sock->socket, FIONREAD, &size);
	return size;
}

void o_database_socket_close(struct o_database_socket * sock)
{
	shutdown(sock->socket, SHUT_RDWR);
	o_free(sock);
}
