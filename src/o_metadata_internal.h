#ifndef O_METADATA_INTERNAL_H_
#define O_METADATA_INTERNAL_H_
#include "o_metadata.h"
#include "o_document.h"

/*! \brief Retrieve the metadata from a document.
 *
 * \param doc where read the metadata.
 * \return a new metadata structure filled with document data.
 */
struct o_metadata * o_metadata_from_document(struct o_document *doc);

/*! \brief Free the matadata used memory with all chindren.
 *
 * \param metadata to free,
 */
void o_metadata_free(struct o_metadata * metadata);

#endif /* O_METADATA_INTERNAL_H_ */
