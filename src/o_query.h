#ifndef O_QUERY_H_
#define O_QUERY_H_
struct o_query;

/*! \brief Create an sql query.
 *
 * \param query the sql to put in the query.
 */
struct o_query * o_query_sql(char * query);

/*! \brief Free the query structure.
 *
 * \param query to free.
 */
void o_query_free(struct o_query * query);

#endif /* O_QUERY_H_ */
