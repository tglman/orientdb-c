#include "o_list.h"
#include "o_memory.h"
#include <string.h>

struct o_list_item
{
	struct o_list_item * next;
	struct o_list_item * previus;
	void * value;
};

struct o_list
{
	struct o_list_item * first;
	struct o_list_item * last;
	int size;
};

struct o_list *o_list_new()
{
	struct o_list * new_l = o_malloc(sizeof(struct o_list));
	memset(new_l, 0, sizeof(struct o_list));
	return new_l;
}

int o_list_add(struct o_list * list, void * to_add)
{
	struct o_list_item * new_i = o_malloc(sizeof(struct o_list_item));
	memset(new_i, 0, sizeof(struct o_list_item));
	new_i->value = to_add;
	list->size++;
	if (list->first == 0)
	{
		list->first = new_i;
		list->last = new_i;
	}
	else
	{
		list->last->next = new_i;
		new_i->previus = list->last;
		list->last = new_i;
	}
	return 1;
}

struct o_list_item * o_list_find_item(struct o_list * list, void *to_find)
{
	struct o_list_item *i = list->first;
	while (i != 0)
	{
		if (i->value == to_find)
			return i;
		i = i->next;
	}
	return 0;
}

int o_list_remove(struct o_list * list, void *to_remove)
{
	struct o_list_item * finded = o_list_find_item(list, to_remove);
	if (finded != 0)
	{
		if (list->first == finded)
			list->first = finded->next;
		if (list->last == finded)
			list->first = finded->previus;
		if (finded->next != 0)
			finded->next->previus = finded->previus;
		if (finded->previus != 0)
			finded->previus->next = finded->next;
		o_free(finded);
		list->size--;
		return 1;
	}
	return 0;
}

int o_list_contains(struct o_list * list, void *to_check)
{
	if (o_list_find_item(list, to_check) != 0)
		return 1;
	else
		return 0;
}

int o_list_size(struct o_list * list)
{
	return list->size;
}

void * o_list_get(struct o_list * list, int pos)
{
	if (pos < 0 || pos >= list->size)
		return 0;
	int count;
	struct o_list_item *i = list->first;
	while (count != pos)
	{
		i = i->next;
		count++;
	}
	return i;
}

void o_list_free(struct o_list * list)
{
	struct o_list_item *i = list->first;
	while (i != 0)
	{
		struct o_list_item *to_free = i;
		i = i->next;
		o_free(to_free);
	}
	o_free(list);
}
