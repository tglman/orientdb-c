#ifndef O_STRING_PRINTER_STREAM_H_
#define O_STRING_PRINTER_STREAM_H_
#include "o_string_printer.h"
#include "o_output_stream.h"

/*! \brief Create a string printer on an stream.
 *
 * \param stream where print.
 */
struct o_string_printer * o_string_printer_stream_new(struct o_output_stream * stream);

#endif /* O_STRING_PRINTER_STREAM_H_ */
