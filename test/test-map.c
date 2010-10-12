#include "../src/o_map.h"
#include <TestFramework/test.h>
#include "test-map.h"


void o_map_test()
{
	struct o_map *mm = o_map_new();
	o_map_put(mm,"aaa",(void *)20);
	void * val = o_map_get(mm,"aaa");
	assert_true(val==(void *)20,"Error on retrieving value from map ");
	o_map_free(mm);
}

void o_map_suite()
{
	ADD_TEST(o_map_test,"creation and put test");
}

