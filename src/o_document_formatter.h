#ifndef O_DOCUMENT_FORMATTER_H_
#define O_DOCUMENT_FORMATTER_H_
#include "o_document.h"
#include "o_input_stream.h"
#include "o_output_stream.h"

struct o_document_formatter;

/*! \brief Serialize a document with current formatter to stream.
 *
 * \param fm formatter to use.
 * \param doc to serialize.
 * \param output where write.
 */
void o_document_formatter_serialize(struct o_document_formatter * fm, struct o_document * doc, struct o_output_stream * output);

/*! \brief Fill a document with content read from stream using the current formatter.
 *
 * \param fm formatter to use.
 * \param doc to fill.
 * \param input to read.
 */
void o_document_formatter_deserialize(struct o_document_formatter * fm, struct o_document * doc, struct o_input_stream * input);

/*! \brief Create and retrieve the default formatter instance.
 *
 * \return the default formatter instance.
 */
struct o_document_formatter * o_document_formatter_factory_default();

/*! \brief Free resources used by formatter.
 *
 *\param formatter to free.
 */
void o_document_formatter_free(struct o_document_formatter * fm);

#endif /* O_DOCUMENT_FORMATTER_H_ */
