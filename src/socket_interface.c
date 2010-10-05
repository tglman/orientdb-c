#include "socket_interface.h"
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
	struct sockaddr_in sock_info;
	//TODO: Retrieve the socket information by the name of site.
	struct hostent *he = gethostbyname(site);
	struct o_database_socket *sock = o_malloc(sizeof(struct o_database_socket));
	sock->socket = socket(AF_INET, SOCK_STREAM, 0);
	sock_info.sin_family = AF_INET;
	sock_info.sin_port = htons(port);
	memcpy(&sock_info.sin_addr, he->h_addr, he->h_length);
	int res = connect(sock->socket, (struct sockaddr*) &sock_info, sizeof(sock_info));
	if (res == -1)
		throw(o_exception_io_new("error on open connection",3));
	return sock;
}

void o_database_socket_send(struct o_database_socket * sock, char * buff, int buff_size)
{
	send(sock->socket, buff, buff_size, 0);
}

void o_database_socket_recv(struct o_database_socket * sock, char * buff, int buff_size)
{
	recv(sock->socket, buff, buff_size, 0);
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
