#include "test_o_native_socket_selector.h"
#include <ctesf.h>
#include "../src/o_native_socket.h"
#include "../src/o_native_socket_selector.h"
#include "../src/o_exceptions.h"
#include "../src/o_native_thread.h"
#include "../src/o_memory.h"
#include <stdio.h>

void *o_native_write_thread(void * par)
{
	struct o_native_socket * cur_sock = 0;
	struct o_native_socket * sock_listen = 0;
	try
	{

		sock_listen = o_native_socket_listen("127.0.0.1", 2000);
		cur_sock = o_native_socket_accept(sock_listen);
	}
	catch(struct o_exception, ex)
	{
		printf("fail listen:%s", o_exception_message(ex));
		o_free(ex);
		fflush(stdout);
		return 0;
	}
	end_try;

	int val;
	int size = sizeof(int);
	o_native_socket_recv(cur_sock, &val, &size, 0);
	o_native_thread_current_sleep(10);
	o_native_socket_send(cur_sock, &val, size);
	o_native_socket_close(cur_sock);
	o_native_socket_close(sock_listen);
	return 0;
}

void o_native_socket_selector_simple_select()
{
	struct o_native_thread * accept_th = o_native_thread_new("thread", o_native_write_thread);
	o_native_thread_start(accept_th);
	o_native_thread_current_sleep(10);
	struct o_native_socket * sock = 0;
	try
	{
		sock = o_native_socket_connect("127.0.0.1", 2000);
	}
	catch(struct o_exception, ex)
	{
		fail("failed connect");
		o_free(ex);
	}
	end_try;
	int val = 10;
	struct o_native_socket_selector * sel = o_native_socket_selector_new();
	o_native_socket_selector_add_socket(sel, sock);
	o_native_socket_send(sock, &val, sizeof(int));
	struct o_native_socket * ret_sock = o_native_socket_selector_select(sel, -1);
	struct o_native_socket * ret_sock1 = o_native_socket_selector_select(sel, 10);
	o_native_thread_current_sleep(10);
	int size = sizeof(int);
	val = 0;
	o_native_socket_recv(ret_sock, &val, &size, 0);
	assert_true(val == 10, "Readed value not the same of send");
	assert_false(ret_sock == 0, "The selector fail");
	assert_false(ret_sock == ret_sock1, "Error select return two time same socket.");
	assert_true(ret_sock1 == 0, "The selector is valid");
	o_native_socket_close(ret_sock);
}

void o_native_socket_selector_suite()
{
	ADD_TEST(o_native_socket_selector_simple_select, "Test an simple select operation on a socket.");
}
