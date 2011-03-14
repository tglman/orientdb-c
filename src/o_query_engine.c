#include "o_query_engine.h"
#include "o_query_engine_internal.h"

void o_query_engine_query(struct o_query_engine * engine, struct o_query * query, void * add_info, query_result_callback callback)
{
	o_query_engine_query_parameter(engine, query, 0, add_info, callback);
}

void o_query_engine_query_parameter(struct o_query_engine * engine, struct o_query * query, struct o_document_value ** parameters, void * add_info,
		query_result_callback callback)
{
	engine->o_query_engine_query_parameter(engine, query, parameters, add_info, callback);
}

void o_query_engine_free(struct o_query_engine * engine)
{
	engine->o_query_engine_free(engine);
}
