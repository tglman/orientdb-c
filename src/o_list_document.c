#include "o_list_document.h"
#include "o_list.h"
#include "o_memory.h"

struct o_list_document
{
	struct o_list *list;
};

struct o_list_document * o_list_document_new()
{
	struct o_list_document * list_doc = o_malloc(sizeof(struct o_list_document));
	list_doc->list = o_list_new();
	return list_doc;
}

void o_list_document_add(struct o_list_document * list, struct o_document * to_add)
{
	if (to_add == 0)
		return;
	o_document_refer(to_add);
	o_list_add(list->list, to_add);
}

struct o_document * o_list_document_get(struct o_list_document * list, int pos)
{
	struct o_document * doc = o_list_get(list->list, pos);
	o_document_refer(doc);
	return doc;
}

int o_list_document_size(struct o_list_document * list)
{
	return o_list_size(list->list);
}

void o_list_document_free(struct o_list_document * list)
{
	struct o_list_iterator * iter = o_list_begin(list->list);
	if (iter != 0)
	{
		do
		{
			struct o_document *doc = (struct o_document *) o_list_iterator_current(iter);
			o_document_release(doc);
		} while (o_list_iterator_next(iter));
		o_list_iterator_free(iter);
	}
	o_list_free(list->list);
	o_free(list);
}
