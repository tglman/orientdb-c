#include "o_string_printer.h"
#include "o_string_printer_internal.h"
#include <string.h>
#include <stdio.h>

void o_string_printer_print(struct o_string_printer *printer, char * string)
{
	printer->o_string_printer_print(printer, string);
}

void o_string_printer_print_long(struct o_string_printer *printer, long long long_val)
{
	char val[20];
	sprintf(val, "%Li", long_val);
	o_string_printer_print(printer, val);
}

void o_string_printer_print_double(struct o_string_printer *printer, double double_val)
{
	char val[20];
	sprintf(val, "%g", double_val);
	o_string_printer_print(printer, val);
}

void o_string_printer_print_char(struct o_string_printer *printer, char ch)
{
	char val[2] = { ch, 0 };
	o_string_printer_print(printer, val);
}

void o_string_printer_flush(struct o_string_printer *printer)
{
	printer->o_string_printer_flush(printer);
}

void o_string_printer_free(struct o_string_printer *printer)
{
	printer->o_string_printer_free(printer);
}
