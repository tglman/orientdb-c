#include "test_connection_remote.h"
#include "../src/o_connection_remote.h"
#include "../src/o_database_socket.h"
#include <TestFramework/test.h>


void test_o_connection_open()
{
	struct o_database_socket * socket = o_database_socket_listen("127.0.0.1",40);
	struct o_connection * server = o_connection_remote_new_accept(socket);
	struct o_connection * client = o_connection_remote_new("127.0.0.1",40);
}

void o_connection_remote_suite()
{
	ADD_TEST(test_o_connection_open," test a connection client server open");
}
