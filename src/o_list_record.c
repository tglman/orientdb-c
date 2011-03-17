#include "o_list_record.h"
#include "o_list.h"
#include "o_memory.h"

struct o_list_record
{
	struct o_list *list;
};

struct o_list_record * o_list_record_new()
{
	struct o_list_record * list = o_malloc(sizeof(struct o_list_record));
	list->list = o_list_new();
	return list;
}

int o_list_record_size(struct o_list_record * list)
{
	return o_list_size(list->list);
}

struct o_record * o_list_record_get(struct o_list_record * list, int pos)
{
	struct o_record * rec = (struct o_record *) o_list_get(list->list, pos);
	o_record_refer(rec);
	return rec;
}

void o_list_record_add(struct o_list_record * list, struct o_record * record)
{
	if (record == 0)
		return;
	o_record_refer(record);
	o_list_add(list->list, record);
}

struct o_record * o_list_record_remove(struct o_list_record * list, struct o_record * record)
{
	if (record == 0)
		return 0;
	struct o_record * rec_ret = o_list_remove(list->list, record);
	o_record_release(rec_ret);
	return rec_ret;
}

void o_list_record_release(struct o_list_record * list)
{

	struct o_list_iterator * b = o_list_begin(list->list);
	if (b != 0)
	{
		do
		{
			o_record_release((struct o_record *) o_list_iterator_current(b));
		} while (o_list_iterator_next(b));
	}
	o_list_free(list->list);
}
