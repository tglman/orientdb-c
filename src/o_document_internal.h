#ifndef O_DOCUMENT_INTERNAL_H_
#define O_DOCUMENT_INTERNAL_H_
#include "o_string_printer.h"
#include "o_document.h"

/*! \brief Deserialize a document.
 *
 * \param doc to fill.
 * \param stream where read data.
 * \param 1 if this document is imbedded.
 */
void o_document_deserialize_internal(struct o_document * doc, struct o_input_stream * stream, int embeddd);

/*! \brief Set the class of document using the class name.
 *
 * \parma doc where set the class.
 * \param class_name of class to set.
 */
void o_document_set_class_by_name(struct o_document * doc, char * class_name);

/*! \brief Serialize an document on a a printer.
 *
 * \param doc to serialize.
 * \print where serialize.
 */
void o_document_serialize_printer(struct o_document * doc, struct o_string_printer * print);

#endif /* O_DOCUMENT_INTERNAL_H_ */
