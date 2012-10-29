#include "test_o_pool.h"
#include "../src/o_pool.h"
#include <ctesf.h>
#include "../src/o_memory.h"

void * pool_test_factory(void * obj)
{
	return o_malloc(sizeof(int));
}

void pool_test_free(void * obj, void * to_free)
{
	return o_free(to_free);
}

void o_pool_test_create_destroy()
{
	struct o_pool * pool = o_pool_new(0, pool_test_factory, pool_test_free);
	void * obj = o_pool_get(pool);
	o_pool_release(pool, obj);
	o_pool_free(pool);
}

void o_pool_test_limit_manage()
{
	struct o_pool * pool = o_pool_new_size(0, pool_test_factory, pool_test_free, 2, 10);
	int i = 10;
	void *arr[10];
	while (i-- > 0)
	{
		arr[i] = o_pool_get(pool);
	}
	void * obj = o_pool_get(pool);
	assert_equals(obj, 0, "Overload of pool limit");
	i = 10;
	while (i-- > 0)
	{
		o_pool_release(pool, arr[i]);
	}
	o_pool_free(pool);
}

void o_pool_suite()
{
	ADD_TEST(o_pool_test_create_destroy, "Test simple pool create and destroy");
	ADD_TEST(o_pool_test_limit_manage, "Test pool limits");
}
