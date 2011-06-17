#include "test_o_url_resolver.h"
#include "../src/o_url_resolver.h"
#include "../src/o_memory.h"
#include <string.h>
#include <ctesf.h>
#include <stdio.h>
#define TEST_URL "remote:127.0.0.1:2424/dbname"

void test_o_url_resolve_type()
{
	enum o_url_type type = o_url_resolve_type(TEST_URL);
	assert_true(type == REMOTE, "Wrong url Type");
}

void test_o_url_resolve_information()
{
	char *path;
	char *database;
	enum o_url_type type;
	o_url_resolve_information(TEST_URL, &type, &path, &database);
	assert_true(type == REMOTE, "Wrong url Type");
	assert_true(strcmp(path, "127.0.0.1:2424") == 0, "Wrong path ");
	assert_true(strcmp(database, "dbname") == 0, "Wrong database name");
	o_free(path);
	o_free(database);
}

void test_o_url_resolve_host_port_from_path()
{
	char *host;
	int port;
	int ret = o_url_resolve_host_port_from_path("127.0.0.1:2424", &host, &port);
	assert_true(ret == 0, "wrong return value");
	assert_true(strcmp(host, "127.0.0.1") == 0, "wrong host name");
	assert_true(port == 2424, "wrong port");
	o_free(host);
}

void test_o_url_resolve_host_from_path()
{
	char *host;
	int port;
	int ret = o_url_resolve_host_port_from_path("127.0.0.1", &host, &port);
	assert_true(ret == 1, "wrong return value");
	assert_true(strcmp(host, "127.0.0.1") == 0, "wrong host name");
	o_free(host);
}

void o_url_resolve_suite()
{
	ADD_TEST(test_o_url_resolve_type, "Test a resolve of type from url");
	ADD_TEST(test_o_url_resolve_information, "Test a url resolver information");
	ADD_TEST(test_o_url_resolve_host_port_from_path, "Test a host and port resolve from path");
	ADD_TEST(test_o_url_resolve_host_from_path, "Test an only host resolve from path");
}
