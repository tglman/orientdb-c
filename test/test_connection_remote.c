#include "test_connection_remote.h"
#include "../src/o_connection_remote.h"
#include "../src/o_database_socket.h"
#include <TestFramework/test.h>
#include "../src/o_native_thread.h"
#include "../src/o_memory.h"
#include "../src/o_exceptions.h"
#include <stdio.h>
#include <string.h>
#define HOST_TEST "127.0.0.1"
#define PORT_TEST 3030

void *test_o_connection_private_listen_thread(void * par)
{
	try
	{
		struct o_database_socket * socket = o_database_socket_listen(HOST_TEST, PORT_TEST);
		struct o_connection * server_conn = o_connection_remote_new_accept(socket);

		struct o_connection_remote * server = (struct o_connection_remote *) server_conn;

		int val = o_connection_remote_read_int(server);
		o_connection_remote_write_int(server, val);

		char val_c = o_connection_remote_read_byte(server);
		o_connection_remote_write_byte(server, val_c);

		short val_s = o_connection_remote_read_short(server);
		o_connection_remote_write_short(server, val_s);

		long long val_64 = o_connection_remote_read_long64(server);
		o_connection_remote_write_long64(server, val_64);

		int number;
		char *bytes = o_connection_remote_read_bytes(server, &number);
		o_connection_remote_write_bytes(server, bytes, number);
		o_free(bytes);

		o_connection_free(server_conn);
		o_database_socket_close(socket);
		o_native_thread_current_exit();
	}
	catch (struct o_exception, e)
	{
		char buf[512];
		memset(buf,0,512);
		sprintf(buf, "Error Message: %s", o_exception_message(e));
		fail(buf);
	}
	end_try;

	return par;
}

void test_o_connection_open()
{
	try
	{

		struct o_native_thread * th = o_native_thread_new("server", test_o_connection_private_listen_thread);
		o_native_thread_start(th);

		o_native_thread_current_sleep(400);

		struct o_connection * client_con = o_connection_remote_new(HOST_TEST, PORT_TEST);
		struct o_connection_remote * client = (struct o_connection_remote *) client_con;

		o_connection_remote_write_int(client, 10);
		int val = o_connection_remote_read_int(client);
		assert_true(val == 10, "server: error receiving int");

		o_connection_remote_write_byte(client, 10);
		char val_c = o_connection_remote_read_byte(client);
		assert_true(val_c == 10, "server: error receiving byte");

		o_connection_remote_write_short(client, 10);
		short val_s = o_connection_remote_read_short(client);
		assert_true(val_s == 10, "server: error receiving short");

		o_connection_remote_write_long64(client, 10);
		long long val_64 = o_connection_remote_read_long64(client);
		assert_true(val_64 == 10, "server: error receiving long 64");

		char to_send[15] =
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		o_connection_remote_write_bytes(client, to_send, 15);

		int number;
		char *bytes = o_connection_remote_read_bytes(client, &number);
		assert_true(number == 15, "server: error receiving size of byte array");
		int i;
		for (i = 0; i < 15; ++i)
			assert_true(bytes[i] == to_send[i], "server: error receiving content of byte array");
		o_free(bytes);
		o_connection_free(client_con);
	}
	catch (struct o_exception, e)
	{
		char buf[512];
		sprintf(buf, "Error Message %s", o_exception_message(e));
		fail(buf);
	}
	end_try;
}

void o_connection_remote_suite()
{
	ADD_TEST(test_o_connection_open, " test a connection client server open");
}
