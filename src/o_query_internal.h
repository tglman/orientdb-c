#ifndef O_QUERY_INTERNAL_H_
#define O_QUERY_INTERNAL_H_
#include "o_query.h"
#include "o_output_stream.h"
/*! \brief Serialize a query.
 *
 * \param query the query to serialize
 * \param the output stream to fill with query
 */
void o_query_seriealize(struct o_query * query, struct o_output_stream * stream);

#endif /* O_QUERY_INTERNAL_H_ */
