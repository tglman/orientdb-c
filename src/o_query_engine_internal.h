#ifndef O_QUERY_ENGINE_INTERNAL_H_
#define O_QUERY_ENGINE_INTERNAL_H_
#include "o_query_engine.h"
struct o_query_engine
{
	void (*o_query_engine_query_parameter)(struct o_query_engine * engine, struct o_query * query, struct o_document_value ** parameters, void * add_info,
			query_result_callback callback);
	void (*o_query_engine_free)(struct o_query_engine * engine);
};

#endif /* O_QUERY_ENGINE_INTERNAL_H_ */
