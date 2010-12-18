#ifndef O_STRING_PRINTER_H_
#define O_STRING_PRINTER_H_

struct o_string_printer;

/*! \brief Print an string on printer.
 *
 * \param printer where print.
 * \param string to print.
 */
void o_string_printer_print(struct o_string_printer *printer, char * string);

/*! \brief Print an long integer to printer.
 *
 * \param printer where print.
 * \param long_val to print.
 */
void o_string_printer_print_long(struct o_string_printer *printer, long long long_val);

/*! \brief Print double value to printer.
 *
 * \param printer where print.
 * \param double_val to print.
 */
void o_string_printer_print_double(struct o_string_printer *printer, double double_val);

/*! \brief Print an character to the printer.
 *
 * \param printer where print.
 * \parma ch to print.
 */
void o_string_printer_print_char(struct o_string_printer *printer, char ch);

/*! \brief Flush the content of printer.
 *
 * \param printer to flush.
 */
void o_string_printer_flush(struct o_string_printer *printer);

/*! \brief Free the data aoolocated on th printer.
 *
 * \param printer to free.
 */
void o_string_printer_free(struct o_string_printer *printer);

#endif /* O_STRING_PRINTER_H_ */
