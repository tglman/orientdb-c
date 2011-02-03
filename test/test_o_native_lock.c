#include "test_o_native_lock.h"
#include "../src/o_native_lock.h"
#include "testframework/test.h"

void test_o_native_lock()
{
	struct o_native_lock * lock = o_native_lock_new();
	o_native_lock_lock(lock);
	o_native_lock_unlock(lock);
	o_native_lock_free(lock);
}

void o_native_lock_suite()
{
	ADD_TEST(test_o_native_lock,"native lock create lock unlock free tests.");
}
