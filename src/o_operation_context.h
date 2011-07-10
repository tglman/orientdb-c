#ifndef O_OPERATION_CONTEXT_H_
#define O_OPERATION_CONTEXT_H_
#include "o_transaction.h"
#include "o_record.h"
#include "o_query.h"

struct o_operation_context;

/*! \brief Handler of found record by query.
 *
 * \param add_info additional info.
 * \param record the found record
 */
typedef void (*QueryHandler)(void * add_info, struct o_record * record);

/*! \brief Execute an save to current context.
 *
 * \param context where execute the save.
 * \param record to save
 * \param cluster_name the cluster name where insert the record used only if record is new.
 * \param rid filled with the record id of saved record.
 * \return !=0 if operation is run correctly otherwise 0.
 */
int o_operation_context_save(struct o_operation_context * context, struct o_record * record, char * cluster_name, struct o_record_id ** rid);

/*! \brief delete the record from the current context.
 *
 * \param context where delete the record.
 * \param record to delate
 * \return !=0 if operation is run correctly otherwise 0.
 */
int o_operation_context_delete(struct o_operation_context * context, struct o_record * record);

/*! \brief load a record from the current context.
 *
 * \param context where load the record.
 * \param record_id of record to load.
 * \return the loaded record or 0 if record was not found.
 */
struct o_record * o_operation_context_load(struct o_operation_context * context, struct o_record_id * record_id);

/*!\brief commit the current context.
 *
 * \param context to commit
 * \return !=0 if operation is run correctly otherwise 0.
 */
int o_operation_context_commit(struct o_operation_context * context);

/*! \brief Commit a transaction to the current transaction context.
 *
 * \param context where commit the transaction.
 * \param transaction to commit.
 * \return !=0 if operation is run correctly otherwise 0.
 */
int o_operation_context_commit_transaction(struct o_operation_context * context, struct o_transaction * transaction);

/*! \brief Execute a query on the current execution context.
 *
 * \param context where execute the query.
 * \param query to execute.
 * \param parameters for the query.
 * \param handler the result handler
 * \param handler_add_info handler additional info.
 * \return !=0 if operation is run correctly otherwise 0.
 */
int o_operation_context_query(struct o_operation_context * context, struct o_query * query, void ** parameters, QueryHandler handler, void * handler_add_info);

/*! \brief Release the current operation context and retrieve the parent context.
 *
 * \param context the context to release.
 * \return the parent context.
 */
struct o_operation_context * o_operation_context_release(struct o_operation_context * context);

#endif /* O_OPERATION_CONTEXT_H_ */
