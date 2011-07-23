#ifndef O_TRANSACTION_H_
#define O_TRANSACTION_H_
#include "o_raw_buffer.h"

enum transaction_operation_type
{
	SAVE, REMOVE, LOADED
};

struct o_transaction;

struct o_transaction_entry;

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

/*! \brief retrieve the entries of transaction.
 *
 * \param transaction where retrieve entries.
 * \param size filled with the number of sizes.
 * \return the array of transaction entries.
 */
struct o_transaction_entry ** o_transaction_get_entries(struct o_transaction *transaction, int * size);

#endif /* O_TRANSACTION_H_ */
