#ifndef O_RECORD_FACTORY_H_
#define O_RECORD_FACTORY_H_

#include "o_record.h"

/*! \brief create an o_record from the type.
 *
 * \param record_type the type of record.
 * \return the new record.
 */
struct o_record * o_record_factory(char record_type);

#endif /* O_RECORD_FACTORY_H_ */
