#include "o_operation_context.h"
#include "o_operation_context_internal.h"

int o_operation_context_save(struct o_operation_context * context, struct o_record * record, char * cluster_name, struct o_record_id ** rid)
{
	return context->type->save(context, record, cluster_name, rid);
}

int o_operation_context_cluster_name_id(struct o_operation_context * context, char * cluster_name)
{
	return context->type->cluster_name_id(context, cluster_name);
}

int o_operation_context_delete(struct o_operation_context * context, struct o_record * record)
{
	return context->type->delete(context, record);
}

struct o_record * o_operation_context_load(struct o_operation_context * context, struct o_record_id * record_id)
{
	return context->type->load(context, record_id);
}

int o_operation_context_commit(struct o_operation_context * context)
{
	return context->type->commit(context);
}

int o_operation_context_rollback(struct o_operation_context * context)
{
	return context->type->rollback(context);
}

int o_operation_context_commit_transaction(struct o_operation_context * context, struct o_transaction * transaction)
{
	return context->type->commit_transaction(context, transaction);
}

int o_operation_context_query(struct o_operation_context * context, struct o_query * query, void ** parameters, QueryHandler handler, void * handler_add_info)
{
	return context->type->query(context, query, parameters, handler, handler_add_info);
}

struct o_operation_context * o_operation_context_release(struct o_operation_context * context)
{
	return context->type->release(context);
}
