#include "suites.h"
#include "../src/o_native_lock.h"

START_TEST( test_o_native_lock)
{
	struct o_native_lock * lock = o_native_lock_new();
	o_native_lock_lock(lock);
	o_native_lock_unlock(lock);
	o_native_lock_free(lock);
}
END_TEST

TCase *  o_native_lock_tests()
{
	TCase *tc_core = tcase_create ("o_record");
	tcase_add_test (tc_core, test_o_native_lock);
	return tc_core;
}
