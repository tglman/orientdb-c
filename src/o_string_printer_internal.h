#ifndef O_STRING_PRINTER_INTERNAL_H_
#define O_STRING_PRINTER_INTERNAL_H_

struct o_string_printer
{
	void (*o_string_printer_print)(struct o_string_printer *printer, char * string);
	void (*o_string_printer_flush)(struct o_string_printer *printer);
	void (*o_string_printer_free)(struct o_string_printer *printer);
};

#endif /* O_STRING_PRINTER_INTERNAL_H_ */
