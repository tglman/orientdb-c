#ifndef O_TRANSACTION_H_
#define O_TRANSACTION_H_

struct o_transaction;

#include "o_operation_context.h"

/*! \brief create a new transaction.
 *
 * \param parent the parent operation context.
 * \return the new transaction instance.
 */
struct o_transaction * o_transaction_new(struct o_operation_context * parent);

/*! \brief Cast transaction to operation context.
 *
 * \param transaction to cast
 * \return return the operation context correspondent to transaction.
 */
struct o_operation_context * o_transaction_to_operation_context(struct o_transaction *transaction);

#endif /* O_TRANSACTION_H_ */
