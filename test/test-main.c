/*
 * test-main.c
 *
 *  Created on: 22 Jun 2010
 *      Author: tglman
 */

#include "o_map.h"
#include <stdio.h>

void main()
{
	struct o_map *mm = o_map_new();
	o_map_put(mm,"aaa",(void *)20);
	int val = (int)o_map_get(mm,"aaa");
	printf("%i",val);
	o_map_free(mm);
}
