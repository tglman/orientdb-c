#ifndef O_STORAGE_CONFIGURATION_H_
#define O_STORAGE_CONFIGURATION_H_
#include "o_record_id.h"
#include "o_storage.h"

struct o_storage_configuration;

/*! \brief Retreive the configuration for storage.
 *
 * \param storage of load configuration.
 * \return the configuration of storage.
 */
struct o_storage_configuration * o_storage_configuration_load(struct o_storage * storage);

/*! \brief Retrieve the schema id from the configuration.
 *
 * \param conf where retrieve the schema id.
 * \return the record id of schema for the storage.
 */
struct o_record_id * o_storage_configuration_get_schema(struct o_storage_configuration * conf);

/*! \brief Retrieve the dictionary id from the configuration.
 *
 * \param conf where retrieve the dictionary id.
 * \return the record id of dictionary for the storage.
 */
struct o_record_id * o_storage_configuration_get_dictionary(struct o_storage_configuration * conf);

#endif /* O_STORAGE_CONFIGURATION_H_ */
