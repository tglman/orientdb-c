#include "o_transaction.h"
#include "o_map.h"
#include "o_operation_context_internal.h"
#include "o_record_internal.h"
#include "o_memory.h"
#include <string.h>

enum record_operation_type
{
	SAVE, REMOVE, LOADED
};

struct o_transaction_entry
{
	enum record_operation_type type;
	struct o_record * record;
};

struct o_transaction
{
	struct o_operation_context context;
	struct o_operation_context * parent;
	struct o_map * records;
	int transaction_count;
};

struct o_transaction_query_handler
{
	struct o_transaction * transaction;
	void * wrappend_endler;
	QueryHandler wrappend_endler_function;
};

void o_entry_transaction_create(void ** key, void ** value)
{
	//TODO Verify what inc
	o_record_refer(((struct o_transaction_entry *) *value)->record);
	o_record_id_refer((struct o_record_id *) *key);
}

void o_entry_transaction_free(void ** key, void ** value)
{
	//TODO Verify what dec
	o_record_id_release((struct o_record_id *) *key);
	o_record_release((struct o_record *) *value);
}

struct o_transaction_entry * o_transaction_resolve_entry(struct o_transaction * trans, struct o_record_id *id)
{
	struct o_transaction_entry * entry = (struct o_transaction_entry *) o_map_get(trans->records, id);
	if (entry == 0)
	{
		entry = o_malloc(sizeof(struct o_transaction_entry));
		memset(entry, 0, sizeof(struct o_transaction_entry));
		o_map_put(trans->records, id, entry);
	}
	return entry;
}

void o_transaction_query_handler(void * add_info, struct o_record * record)
{
	struct o_transaction_query_handler * handler = (struct o_transaction_query_handler *) add_info;
	struct o_record_id *id = o_record_get_id(record);
	struct o_transaction_entry * entry = o_transaction_resolve_entry(handler->transaction, id);
	entry->type = LOADED;
	entry->record = record;
	handler->wrappend_endler_function(handler->wrappend_endler, record);
}

int o_transaction_save(struct o_operation_context * context, struct o_record * record, char * cluster_name, struct o_record_id ** rid)
{
	struct o_transaction * trans = (struct o_transaction *) context;
	struct o_record_id *id = o_record_get_id(record);
	if (o_record_id_is_new(id))
	{
		if (o_record_id_record_id(id) == CLUSTER_POS_INVALID)
		{
			id = o_record_id_new(CLUSTER_ID_INVALID, trans->transaction_count++);
			*rid = id;
			o_record_reset_id(record, id);
		}
	}
	struct o_transaction_entry * entry = (struct o_transaction_entry *) o_map_get(trans->records, id);
	if (entry == 0)
	{
		entry = o_malloc(sizeof(struct o_transaction_entry));
		o_map_put(trans->records, id, entry);
	}
	entry->record = record;
	entry->type = SAVE;
	return 1;
}

int o_transaction_delete(struct o_operation_context * context, struct o_record * record)
{
	struct o_transaction * trans = (struct o_transaction *) context;
	struct o_record_id *id = o_record_get_id(record);
	struct o_transaction_entry * entry = o_transaction_resolve_entry(trans, id);
	entry->type = REMOVE;
	if (entry->record != 0)
	{
		//TODO:verify if unrelease already refferred record..
		o_record_release(entry->record);
	}
	entry->record = record;
	o_record_refer(entry->record);
	return 1;
}

struct o_record * o_transaction_load(struct o_operation_context * context, struct o_record_id * record_id)
{
	struct o_transaction * trans = (struct o_transaction *) context;
	return trans->parent->type->load(context, record_id);
}

int o_transaction_commit(struct o_operation_context * context)
{
	struct o_transaction * trans = (struct o_transaction *) context;
	return trans->parent->type->commit_transaction(trans->parent, trans);
}

int o_transaction_commit_transaction(struct o_operation_context * context, struct o_transaction * transaction)
{
	struct o_transaction * trans = (struct o_transaction *) context;
	int size;
	struct o_transaction_entry** entries = (struct o_transaction_entry**) o_map_values(transaction->records, &size);
	while (size-- > 0)
	{
		struct o_record_id *id = o_record_get_id(entries[size]->record);
		struct o_transaction_entry * entry = o_transaction_resolve_entry(trans, id);
		//TODO Verify operation collision
		entry->type = entries[size]->type;
		entry->record = entries[size]->record;
	}
	transaction->context.type->release(&transaction->context);
	return 1;
}

int o_transaction_query(struct o_operation_context * context, struct o_query * query, void ** parameters, QueryHandler handler, void * handler_add_info)
{
	struct o_transaction * trans = (struct o_transaction *) context;
	struct o_transaction_query_handler current_handler;
	current_handler.transaction = trans;
	current_handler.wrappend_endler_function = handler;
	current_handler.wrappend_endler = handler_add_info;
	return trans->parent->type->query(trans->parent, query, parameters, o_transaction_query_handler, &current_handler);
}

struct o_operation_context * o_transaction_release(struct o_operation_context * context)
{
	struct o_transaction * trans = (struct o_transaction *) context;
	o_map_free(trans->records);
	struct o_operation_context * parent = trans->parent;
	o_free(trans);
	return parent;
}

struct o_operation_context_class o_transaction_operation_context_instance =
{ .save = o_transaction_save, .delete = o_transaction_delete, .load = o_transaction_load, .commit = o_transaction_commit, .commit_transaction =
		o_transaction_commit_transaction, .query = o_transaction_query, .release = o_transaction_release };

struct o_operation_context * o_transaction_to_operation_context(struct o_transaction *transaction)
{
	return &transaction->context;
}

struct o_transaction * o_transaction_new(struct o_operation_context * parent)
{
	struct o_transaction * transaction = (struct o_transaction *) o_malloc(sizeof(struct o_transaction));
	transaction->context.type = &o_transaction_operation_context_instance;
	transaction->parent = parent;
	transaction->records = o_map_new((unsigned int(*)(void *)) o_record_id_hash, o_entry_transaction_create, o_entry_transaction_free,
			(int(*)(void *, void *)) o_record_id_compare);
	return transaction;
}
