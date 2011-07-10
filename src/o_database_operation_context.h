#ifndef O_DATABASE_OPERATION_CONTEXT_H_
#define O_DATABASE_OPERATION_CONTEXT_H_
#include "o_operation_context.h"
#include "o_storage.h"

/*! \brief create a new database operation context on an storage.
 *
 * \param storage to wrap.
 * \return new operation context.
 */
struct o_operation_context * o_database_operation_context(struct o_storage * storage);

#endif /* O_DATABASE_OPERATION_CONTEXT_H_ */
