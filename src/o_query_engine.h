#ifndef O_QUERY_ENGINE_H_
#define O_QUERY_ENGINE_H_
#include "o_list.h"
#include "o_query.h"
#include "o_document_value.h"
#include "o_raw_buffer.h"
#include "o_query_callback.h"
struct o_query_engine;

/*! \brief Execute an query with parameters.
 *
 * \param engine to use.
 * \param query to execute.
 * \param add_info give to the callback.
 * \param callback called on result retrieve
 *
 */
void o_query_engine_query(struct o_query_engine * engine, struct o_query * query, void * add_info, query_result_callback callback);

/*! \brief Execute an query with parameters.
 *
 * \param engine to use.
 * \param query to execute.
 * \param parameters to populate the query
 * \param add_info give to the callback.
 * \param callback called on result retrieve
 */
void o_query_engine_query_parameter(struct o_query_engine * engine, struct o_query * query, struct o_document_value ** parameters, void * add_info,
		query_result_callback callback);

/*! \brief Free the structure used by the query engine.
 *
 * \param engine to free.
 */
void o_query_engine_free(struct o_query_engine * engine);

#endif /* O_QUERY_ENGINE_H_ */
