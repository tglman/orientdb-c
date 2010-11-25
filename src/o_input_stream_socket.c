#include "o_input_stream_socket.h"
#include "o_input_stream_internal.h"
#include "o_memory.h"

struct o_input_stream_socket
{
	struct o_input_stream stream;
	struct o_database_socket * socket;
};

int o_input_stream_socket_read(struct o_input_stream * stream)
{
	struct o_input_stream_socket* in_sock = (struct o_input_stream_socket *) stream;
	unsigned char c;
	int size = 1;
	o_database_socket_recv(in_sock->socket, &c, &size, 0);
	if (size == 0)
		return -1;
	return c;
}
int o_input_stream_socket_peek(struct o_input_stream * stream)
{
	struct o_input_stream_socket* in_sock = (struct o_input_stream_socket *) stream;
	unsigned char c;
	int size = 1;
	o_database_socket_recv(in_sock->socket, &c, &size, READ_PEEK);
	if (size == 0)
		return -1;
	return c;
}

int o_input_stream_socket_read_bytes(struct o_input_stream * stream, void * bytes, int size)
{
	struct o_input_stream_socket* in_sock = (struct o_input_stream_socket*) stream;
	o_database_socket_recv(in_sock->socket, bytes, &size, 0);
	return size;
}

void o_input_stream_socket_free(struct o_input_stream * stream)
{
	o_free(stream);
}

struct o_input_stream * o_input_stream_socket_new(struct o_database_socket * socket)
{
	struct o_input_stream_socket * new_str = o_malloc(sizeof(struct o_input_stream_socket));
	new_str->socket = socket;
	new_str->stream.o_input_stream_read = o_input_stream_socket_read;
	new_str->stream.o_input_stream_peek = o_input_stream_socket_peek;
	new_str->stream.o_input_stream_read_bytes = o_input_stream_socket_read_bytes;
	new_str->stream.o_input_stream_free = o_input_stream_socket_free;

	return (struct o_input_stream*) new_str;
}
