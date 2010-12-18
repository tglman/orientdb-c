#include "o_string_printer_stream.h"
#include "o_string_printer_internal.h"
#include "o_memory.h"
#include <string.h>

struct o_string_printer_stream
{
	struct o_string_printer printer;
	struct o_output_stream * stream;
};

void o_string_printer_stream_print(struct o_string_printer * printer, char * string)
{
	struct o_string_printer_stream * print_stream = (struct o_string_printer_stream *) printer;
	o_output_stream_write_bytes(print_stream->stream, string, strlen(string));
}

void o_string_printer_stream_flush(struct o_string_printer * printer)
{
	struct o_string_printer_stream * print_stream = (struct o_string_printer_stream *) printer;
	o_output_stream_flush(print_stream->stream);
}

void o_string_printer_stream_free(struct o_string_printer * printer)
{
	o_free(printer);
}

struct o_string_printer * o_string_printer_stream_new(struct o_output_stream * stream)
{
	struct o_string_printer_stream * print_stream = o_malloc(sizeof(struct o_string_printer_stream));
	print_stream->stream = stream;
	print_stream->printer.o_string_printer_print = o_string_printer_stream_print;
	print_stream->printer.o_string_printer_flush = o_string_printer_stream_flush;
	print_stream->printer.o_string_printer_free = o_string_printer_stream_free;

	return &print_stream->printer;

}
