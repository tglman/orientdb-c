#ifndef O_DOCUMENT_INTERNAL_H_
#define O_DOCUMENT_INTERNAL_H_
#include "o_string_printer.h"

/*! \brief Deserialize a document.
 *
 * \param doc to fill.
 * \param stream where read data.
 * \param 1 if this document is imbedded.
 */
void o_document_deserialize_internal(struct o_document * doc, struct o_input_stream * stream, int embeddd);

/*! \brief Serialize an document on a a printer.
 *
 * \param doc to serialize.
 * \print where serialize.
 */
void o_document_serialize_printer(struct o_document * doc, struct o_string_printer * print);

#endif /* O_DOCUMENT_INTERNAL_H_ */
