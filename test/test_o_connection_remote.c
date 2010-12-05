#include "test_o_connection_remote.h"
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
		unsigned char *bytes = o_connection_remote_read_bytes(server, &number);
		o_connection_remote_write_bytes(server, (unsigned char *)bytes, number);
		o_free(bytes);

		char * string = o_connection_remote_read_string(server);
		o_connection_remote_write_string(server, string);
		o_free(string);

		o_connection_free(server_conn);
		o_database_socket_close(socket);
		o_native_thread_current_exit();
	}
	catch (struct o_exception, e)
	{
		char buf[512];
		printf(buf, "Thread fail unsupported -> Error Message: %s", o_exception_message(e));
		fflush(stdout);
	}
	end_try;

	return par;
}

void test_o_connection_local_data_transfer()
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
		assert_true(val == 10, "error transfer int");

		o_connection_remote_write_byte(client, 10);
		char val_c = o_connection_remote_read_byte(client);
		assert_true(val_c == 10, "error transfer byte");

		o_connection_remote_write_short(client, 10);
		short val_s = o_connection_remote_read_short(client);
		assert_true(val_s == 10, "error transfer short");

		o_connection_remote_write_long64(client, 10);
		long long val_64 = o_connection_remote_read_long64(client);
		assert_true(val_64 == 10, "error transfer long 64");

		unsigned char to_send[15] =
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		o_connection_remote_write_bytes(client, to_send, 15);

		int number;
		unsigned char *bytes = o_connection_remote_read_bytes(client, &number);
		assert_true(number == 15, "error transfer size of byte array");
		int i;
		for (i = 0; i < 15; ++i)
			assert_true(bytes[i] == to_send[i], "error transfer content of byte array");
		o_free(bytes);

		char * string = "test string";
		o_connection_remote_write_string(client, string);
		char * string_ret = o_connection_remote_read_string(client);
		assert_true(strcmp(string, string_ret) == 0, "error transfer string");
		o_free(string_ret);

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

void test_o_connection_remote_data_transfer()
{
	try
	{

		struct o_connection * client_con = o_connection_remote_new(HOST_TEST, 3333);
		struct o_connection_remote * client = (struct o_connection_remote *) client_con;

		o_connection_remote_write_int(client, 10);
		int val = o_connection_remote_read_int(client);
		assert_true(val == 10, "error transfer int");

		o_connection_remote_write_byte(client, 10);
		char val_c = o_connection_remote_read_byte(client);
		assert_true(val_c == 10, "error transfer byte");

		o_connection_remote_write_short(client, 10);
		short val_s = o_connection_remote_read_short(client);
		assert_true(val_s == 10, "error transfer short");

		o_connection_remote_write_long64(client, 10);
		long long val_64 = o_connection_remote_read_long64(client);
		assert_true(val_64 == 10, "error transfer long 64");

		unsigned char to_send[15] =
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		o_connection_remote_write_bytes(client, to_send, 15);

		int number;
		unsigned char *bytes = o_connection_remote_read_bytes(client, &number);
		assert_true(number == 15, "error transfer size of byte array");
		int i;
		for (i = 0; i < 15; ++i)
			assert_true(bytes[i] == to_send[i], "error transfer content of byte array");
		o_free(bytes);

		char * string = "test string";
		o_connection_remote_write_string(client, string);
		char * string_ret = o_connection_remote_read_string(client);
		assert_true(strcmp(string, string_ret) == 0, "error transfer string");
		o_free(string_ret);

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
	//ADD_TEST(test_o_connection_local_data_transfer, " test a connection client server data transfer");
	//Remote test commented for unaviable server.
	//ADD_TEST(test_o_connection_remote_data_transfer, " test a connection to java server with data transfer");
}
