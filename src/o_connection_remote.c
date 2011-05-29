#include "o_connection_remote.h"
#include "o_native_socket.h"
#include "o_storage_remote.h"
#include "o_input_stream_socket.h"
#include "o_output_stream_socket.h"
#include "o_native_lock.h"
#include "o_memory.h"
#include <string.h>
#include <netinet/in.h>
#include "o_exceptions.h"

#define CURRENT_PROTOCOL 3

struct o_connection_remote
{
	struct o_native_socket *socket;
	struct o_input_stream * input;
	struct o_output_stream * output;
	struct o_native_lock * input_lock;
	struct o_native_lock * cond_lock;
	struct o_native_cond * cond;
	int session_id;
	char response_id;
	char readed;
};


struct o_connection_remote * o_connection_remote_new_accept(struct o_native_socket * listen_sock)
{
	struct o_connection_remote * conn = o_malloc(sizeof(struct o_connection_remote));
	memset(conn, 0, sizeof(struct o_connection_remote));
	conn->socket = o_native_socket_accept(listen_sock);
	conn->input = o_input_stream_socket_new(conn->socket);
	conn->output = o_output_stream_socket_new(conn->socket);
	conn->input_lock = o_native_lock_new();
	conn->cond_lock = o_native_lock_new();
	conn->cond = o_native_cond_new();
	return conn;
}

struct o_connection_remote * o_connection_remote_new(char * host, int port)
{
	struct o_connection_remote * conn = o_malloc(sizeof(struct o_connection_remote));
	memset(conn, 0, sizeof(struct o_connection_remote));
	conn->socket = o_native_socket_connect(host, port);
	conn->input = o_input_stream_socket_new(conn->socket);
	conn->output = o_output_stream_socket_new(conn->socket);
	conn->input_lock = o_native_lock_new();
	conn->cond_lock = o_native_lock_new();
	conn->cond = o_native_cond_new();

	if (o_connection_remote_read_short(conn) != CURRENT_PROTOCOL)
		throw(o_exception_new("Wrong protocol version", 1));
	return conn;
}

int o_connection_remote_read_int(struct o_connection_remote * connection)
{
	int toRead = 0;
	o_input_stream_read_bytes(connection->input, &toRead, sizeof(toRead));
	return ntohl(toRead);
}

long long o_connection_remote_read_long64(struct o_connection_remote * connection)
{
	long long ret_val = 0;
	o_input_stream_read_bytes(connection->input, &ret_val, sizeof(ret_val));
	ret_val = (((long long) (ntohl((int) ((ret_val << 32) >> 32))) << 32) | (unsigned int) ntohl(((int) (ret_val >> 32))));
	return ret_val;
}

char o_connection_remote_read_byte(struct o_connection_remote * connection)
{
	char toRead = 0;
	o_input_stream_read_bytes(connection->input, &toRead, sizeof(toRead));
	return toRead;
}

short o_connection_remote_read_short(struct o_connection_remote * connection)
{
	short toRead = 0;
	o_input_stream_read_bytes(connection->input, &toRead, sizeof(toRead));
	return ntohs(toRead);
}

unsigned char * o_connection_remote_read_bytes(struct o_connection_remote * connection, int *byte_read)
{
	*byte_read = o_connection_remote_read_int(connection);
	if (*byte_read > 0)
	{
		unsigned char * bytes = o_malloc(*byte_read * sizeof(unsigned char));
		o_input_stream_read_bytes(connection->input, bytes, *byte_read);
		return bytes;
	}
	return 0;
}

char * o_connection_remote_read_string(struct o_connection_remote * connection)
{
	int byte_to_read = o_connection_remote_read_int(connection);
	char * bytes = o_malloc((byte_to_read + 1) * sizeof(char));
	o_input_stream_read_bytes(connection->input, bytes, byte_to_read);
	bytes[byte_to_read] = 0;
	return bytes;
}

char ** o_connection_remote_read_array_strings(struct o_connection_remote * connection, int *array_size)
{
	*array_size = o_connection_remote_read_int(connection);
	char **string_array = o_malloc(*array_size * sizeof(char *));
	int i;
	for (i = 0; i < *array_size; i++)
		string_array[i] = o_connection_remote_read_string(connection);
	return string_array;
}

void o_connection_remote_write_int(struct o_connection_remote * connection, int int_value)
{
	int_value = htonl(int_value);
	o_output_stream_write_bytes(connection->output, &int_value, sizeof(int_value));
}

void o_connection_remote_write_long64(struct o_connection_remote * connection, long long long_value)
{
	long_value = (((long long) (htonl((int) ((long_value << 32) >> 32))) << 32) | (unsigned int) htonl(((int) (long_value >> 32))));
	o_output_stream_write_bytes(connection->output, &long_value, sizeof(long_value));
}

void o_connection_remote_write_byte(struct o_connection_remote * connection, char byte_value)
{
	o_output_stream_write_bytes(connection->output, &byte_value, sizeof(byte_value));
}

void o_connection_remote_write_short(struct o_connection_remote * connection, short short_value)
{
	short_value = htons(short_value);
	o_output_stream_write_bytes(connection->output, &short_value, sizeof(short_value));
}

void o_connection_remote_write_bytes(struct o_connection_remote * connection, unsigned char *byte_array, int length)
{
	o_connection_remote_write_int(connection, length);
	o_output_stream_write_bytes(connection->output, byte_array, length);
}

void o_connection_remote_write_string(struct o_connection_remote * connection, char *string)
{
	o_connection_remote_write_bytes(connection, (unsigned char *) string, strlen(string));
}

void o_connection_remote_flush(struct o_connection_remote * connection)
{
	o_output_stream_flush(connection->output);
}

void o_connection_remote_write_array_strings(struct o_connection_remote * connection, char **strings_array, int length)
{
	o_connection_remote_write_int(connection, length);
	int i;
	for (i = 0; i < length; i++)
		o_connection_remote_write_string(connection, strings_array[i]);
}

int o_connection_remote_begin_read_session(struct o_connection_remote * connection, int session_id)
{
	do
	{
		o_native_lock_lock(connection->input_lock);
		if (!connection->readed)
		{
			connection->readed = 1;
			connection->response_id = o_connection_remote_read_byte(connection);
			connection->session_id = o_connection_remote_read_int(connection);
		}
		if (connection->session_id == session_id)
		{
			connection->readed = 0;
			return connection->response_id;
		}

		o_native_lock_unlock(connection->input_lock);
		o_native_lock_lock(connection->cond_lock);
		o_native_cond_wait(connection->cond, connection->cond_lock);
		o_native_lock_unlock(connection->cond_lock);
	} while (1);
	/*never executed remove only warning.*/
	return 0;
}

void o_connection_remote_end_read(struct o_connection_remote * connection)
{
	connection->readed = 0;
	o_native_lock_unlock(connection->input_lock);
	o_native_lock_lock(connection->cond_lock);
	o_native_cond_broadcast(connection->cond);
	o_native_lock_unlock(connection->cond_lock);
}

void o_connection_remote_free(struct o_connection_remote *connection)
{
	o_native_socket_close(connection->socket);
	o_input_stream_free(connection->input);
	o_output_stream_free(connection->output);
	o_native_lock_free(connection->input_lock);
	o_native_lock_free(connection->cond_lock);
	o_native_cond_free(connection->cond);
	o_free(connection);
}

