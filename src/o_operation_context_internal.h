#ifndef O_OPERATION_CONTEXT_INTERNAL_H_
#define O_OPERATION_CONTEXT_INTERNAL_H_
#include "o_operation_context.h"

struct o_operation_context_class
{
	int (*save)(struct o_operation_context * context, struct o_record * record, char * cluster_name, struct o_record_id ** rid);
	int (*delete)(struct o_operation_context * context, struct o_record * record);
	struct o_record * (*load)(struct o_operation_context * context, struct o_record_id * record_id);
	int (*commit)(struct o_operation_context * context);
	int (*rollback)(struct o_operation_context * context);
	int (*commit_transaction)(struct o_operation_context * context, struct o_transaction * transaction);
	int (*query)(struct o_operation_context * context, struct o_query * query, void ** parameters, QueryHandler handler, void * handler_add_info);
	int (*cluster_name_id)(struct o_operation_context * context, char * cluster_name);
	struct o_operation_context * (*release)(struct o_operation_context * context);
};

struct o_operation_context
{
	struct o_operation_context_class *type;
};

#endif /* O_OPERATION_CONTEXT_INTERNAL_H_ */
