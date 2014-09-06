#include "suites.h"
#include "../src/o_url_resolver.h"
#include "../src/o_memory.h"
#include <string.h>
#include <stdio.h>
#define TEST_URL "remote:127.0.0.1:2424/dbname"

START_TEST( test_o_url_resolve_type)
{
	enum o_url_type type = o_url_resolve_type(TEST_URL);
	assert_true(type == REMOTE, "Wrong url Type");
}
END_TEST

START_TEST (test_o_url_resolve_information)
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
END_TEST

START_TEST( test_o_url_resolve_host_port_from_path)
{
	char *host;
	int port;
	int ret = o_url_resolve_host_port_from_path("127.0.0.1:2424", &host, &port);
	assert_true(ret == 0, "wrong return value");
	assert_true(strcmp(host, "127.0.0.1") == 0, "wrong host name");
	assert_true(port == 2424, "wrong port");
	o_free(host);
}
END_TEST

START_TEST( test_o_url_resolve_host_from_path)
{
	char *host;
	int port;
	int ret = o_url_resolve_host_port_from_path("127.0.0.1", &host, &port);
	assert_true(ret == 1, "wrong return value");
	assert_true(strcmp(host, "127.0.0.1") == 0, "wrong host name");
	o_free(host);
}
END_TEST

TCase * o_url_resolve_tests()
{
	TCase *tc_core = tcase_create ("o_url_resolve");
	tcase_add_test (tc_core, test_o_url_resolve_type);
	tcase_add_test (tc_core, test_o_url_resolve_information);
	tcase_add_test (tc_core, test_o_url_resolve_host_port_from_path);
	tcase_add_test (tc_core, test_o_url_resolve_host_from_path);
	return tc_core;
}
