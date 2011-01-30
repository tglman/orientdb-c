#ifndef O_METADATA_H_
#define O_METADATA_H_
#include "o_schema.h"

struct o_metadata;

/*! \brief Retrieve the schema from the metadata.
 *
 * \param metadata where retrieve the schema.
 * \return the schema of metadata.
 */
struct o_schema * o_metadata_get_schema(struct o_metadata * metadata);

#endif /* O_METADATA_H_ */
