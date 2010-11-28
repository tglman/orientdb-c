#ifndef O_DOCUMENT_INTERNAL_H_
#define O_DOCUMENT_INTERNAL_H_

/*! \brief Deserialize a document.
 *
 * \param doc to fill.
 * \param stream where read data.
 * \param 1 if this document is imbedded.
 */
void o_document_deserialize_internal(struct o_document * doc, struct o_input_stream * stream,int embeddd);

#endif /* O_DOCUMENT_INTERNAL_H_ */
