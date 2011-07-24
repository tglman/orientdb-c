#ifndef O_TRANSACTION_INTERNAL_H_
#define O_TRANSACTION_INTERNAL_H_
#include "o_transaction.h"
#include "o_raw_buffer.h"

/*! \brief Retrieve operation type of entry.
 *
 * \param entry where retrieve type.
 * \return the type of entry.
 */
enum transaction_operation_type o_transaction_entry_get_type(struct o_transaction_entry * entry);

/*! \brief Retrieve the raw buffer of current transaction entry.
 *
 * \param entry where retrieve raw buffer.
 * \return the raw buffer for entry.
 */
struct o_raw_buffer * o_transaction_entry_get_raw_buffer(struct o_transaction_entry * entry);

/*! \brief Retrieve the record id for current entry.
 *
 * \param entry the where retrieve the rid.
 * \return the id of current entry.
 */
struct o_record_id * o_transaction_entry_get_rid(struct o_transaction_entry * entry);

/*! \brief Update the record in the transaction with updated id after the commit.
 *
 * \param transaction the transaction where update record.
 * \param old the old id.
 * \param new the new id.
 */
void o_transaction_update_id(struct o_transaction * transaction, struct o_record_id *old, struct o_record_id * new);



#endif /* O_TRANSACTION_INTERNAL_H_ */
