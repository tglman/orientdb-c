#include "o_document_formatter_standard.h"
#include "o_document_formatter_internal.h"
#include "o_document_value.h"
#include "o_document_internal.h"
#include "o_string_buffer.h"
#include "o_memory.h"
#include "o_exceptions.h"
#include "o_input_stream.h"
#include "o_string_printer.h"
#include "o_string_printer_stream.h"
#include <stdio.h>
#include <string.h>

struct o_document_formatter_standard
{
	struct o_document_formatter formatter;
};

struct o_document_value * o_doc_for_std_value_deserialize(struct o_input_stream * stream);
void o_doc_for_std_serialize_printer(struct o_document_formatter * fm, struct o_document * doc, struct o_string_printer * printer);

void o_document_formatter_standard_free(struct o_document_formatter * fm)
{
	o_free(fm);
}

void o_document_formatter_value_serialize(struct o_document_formatter * fm, struct o_document_value * o_value, struct o_string_printer *buff)
{
	switch (o_document_value_type(o_value))
	{
	case BOOL:
		if (o_document_value_get_bool(o_value))
			o_string_printer_print(buff, "true");
		else
			o_string_printer_print(buff, "false");
		break;
	case STRING:
		o_string_printer_print(buff, "\"");
		char * val = o_document_value_get_string(o_value);
		while (*val != 0)
		{
			if (*val == '"' || *val == '\'' || *val == '\\')
				o_string_printer_print_char(buff, '\\');
			o_string_printer_print_char(buff, *val++);
		}
		o_string_printer_print(buff, "\"");
		break;
	case BYTE:
		o_string_printer_print_long(buff, o_document_value_get_byte(o_value));
		o_string_printer_print_char(buff, 'b');
		break;
	case INT:
		o_string_printer_print_long(buff, o_document_value_get_int(o_value));
		o_string_printer_print_char(buff, 'i');
		break;
	case LONG:
		o_string_printer_print_long(buff, o_document_value_get_long(o_value));
		o_string_printer_print_char(buff, 'l');
		break;
	case SHORT:
		o_string_printer_print_long(buff, o_document_value_get_short(o_value));
		o_string_printer_print_char(buff, 's');
		break;
	case FLOAT:
		o_string_printer_print_double(buff, o_document_value_get_float(o_value));
		o_string_printer_print_char(buff, 'f');
		break;
	case DOUBLE:
		o_string_printer_print_double(buff, o_document_value_get_double(o_value));
		o_string_printer_print_char(buff, 'd');
		break;
	case DATE:
		o_string_printer_print_long(buff, o_document_value_get_date(o_value));
		o_string_printer_print_char(buff, 't');
		break;
	case EMBEDDED:
		o_string_printer_print(buff, "(");
		o_doc_for_std_serialize_printer(fm, o_document_value_get_embedded(o_value), buff);
		o_string_printer_print(buff, ")");
		break;
	case LINK:
	{
		struct o_record_id * id = o_document_value_get_link_ref(o_value);
		if (id != 0)
		{
			o_string_printer_print_char(buff, '#');
			o_string_printer_print(buff, o_record_id_string(id));
		}
	}
		break;

	case ARRAY:
	{
		o_string_printer_print(buff, "[");
		int s;
		int i;
		struct o_document_value ** array = o_document_value_get_array(o_value, &s);
		for (i = 0; i < s; i++)
		{
			o_document_formatter_value_serialize(fm, array[i], buff);
			if (i < s)
				o_string_printer_print(buff, ",");
		}
		o_string_printer_print(buff, "]");
	}
		break;
	}
}

void o_doc_for_std_serialize_printer(struct o_document_formatter * fm, struct o_document * doc, struct o_string_printer * printer)
{
	int names_count;
	char ** names = o_document_field_names(doc, &names_count);
	int i;
	char * name = o_document_get_class_name(doc);
	if (name != 0)
	{
		o_string_printer_print(printer, name);
		o_string_printer_print_char(printer, '@');
	}
	for (i = 0; i < names_count; i++)
	{
		o_string_printer_print(printer, names[i]);
		o_string_printer_print(printer, ":");
		o_document_formatter_value_serialize(fm, o_document_field_get(doc, names[i]), printer);
		if (i < names_count - 1)
			o_string_printer_print(printer, ",");
	}
}

void o_document_formatter_standard_serialize(struct o_document_formatter * fm, struct o_document * doc, struct o_output_stream * output)
{
	struct o_string_printer * printer = o_string_printer_stream_new(output);
	o_doc_for_std_serialize_printer(fm, doc, printer);
	o_string_printer_flush(printer);
	o_string_printer_free(printer);
}

struct o_document_value * o_doc_for_std_value_string_deserialize(struct o_input_stream * stream)
{
	int readed;
	struct o_string_buffer * buff = o_string_buffer_new();
	while ((readed = o_input_stream_read(stream)) != '"')
	{
		if (readed == '\\')
			readed = o_input_stream_read(stream);
		o_string_buffer_append_char(buff, readed);
	}
	char * str = o_string_buffer_str(buff);
	struct o_document_value *value = o_document_value_string(str);
	o_free(str);
	o_string_buffer_free(buff);
	return value;
}

struct o_document_value * o_doc_for_std_value_embedded_deserialize(struct o_input_stream * stream)
{
	struct o_document *doc = o_document_new();
	o_document_deserialize(doc, stream);
	struct o_document_value * val = o_document_value_embedded(doc);
	o_document_release(doc);
	return val;
}

struct o_document_value * o_doc_for_std_value_link_deserialize(struct o_input_stream * stream)
{
	int readed;
	int cid = 0;
	long long rid;
	struct o_string_buffer * buff = o_string_buffer_new();
	do
	{
		readed = o_input_stream_peek(stream);
		if (readed == ':')
		{
			char * ca = o_string_buffer_str(buff);
			cid = atol(ca);
			o_free(ca);
			o_string_buffer_clear(buff);
			o_input_stream_read(stream);
		}
		else if (readed >= '0' && readed <= '9')
		{
			o_string_buffer_append_char(buff, readed);
			//Remove the peek from stream
			o_input_stream_read(stream);
		}
		else if (readed != ',' && readed != -1 && readed != ')' && readed != ']' && readed != '*')
		{
			o_string_buffer_free(buff);
			char message[50];
			sprintf(message, "wrong link parsing char:%c", readed);
			throw(o_exception_new(message, 0));
		}
	} while (readed != ',' && readed != -1 && readed != ')' && readed != ']' && readed != '*');
	char * ca = o_string_buffer_str(buff);
	rid = atol(ca);
	o_free(ca);
	o_string_buffer_free(buff);
	struct o_record_id * o_rid = o_record_id_new(cid, rid);
	struct o_document_value * val = o_document_value_link_ref(o_rid);
	o_record_id_release(o_rid);
	return val;
}

struct o_document_value_list
{
	struct o_document_value *val;
	struct o_document_value_list * next;
};

struct o_document_value * o_doc_for_std_value_array_deserialize(struct o_input_stream * stream)
{
	struct o_document_value_list *value_list = 0;
	struct o_document_value_list *root = 0;
	int size = 0;
	struct o_document_value *value = 0;
	int readed;
	do
	{
		struct o_document_value *cur = o_doc_for_std_value_deserialize(stream);
		if (cur != 0)
		{
			if (value_list == 0)
				root = value_list = o_malloc(sizeof(struct o_document_value_list));
			else
				value_list = value_list->next = o_malloc(sizeof(struct o_document_value_list));
			value_list->val = cur;
			size++;
		}
		readed = o_input_stream_peek(stream);
		if (readed == ',' || readed == ']')
			o_input_stream_read(stream);
	} while (readed != ']');

	struct o_document_value ** array = o_malloc(sizeof(struct o_document_value *) * size);
	int i = 0;
	value_list = root;
	while (i < size)
	{
		array[i++] = value_list->val;
		struct o_document_value_list *cur = value_list;
		value_list = value_list->next;
		o_free(cur);
	}
	value = o_document_value_array(array, size);
	o_free(array);
	return value;
}

struct o_document_value * o_doc_for_std_value_bool_deserialize(struct o_input_stream * stream)
{
	int istrue = o_input_stream_read(stream);
	o_input_stream_read(stream);
	o_input_stream_read(stream);
	o_input_stream_read(stream);
	if (istrue == 'f')
	{
		o_input_stream_read(stream);
	}
	return o_document_value_bool(istrue == 't');
}

struct o_document_value * o_doc_for_std_value_plain_factory(char * serialized_content, char type)
{
	double number = atof(serialized_content);
	struct o_document_value *val = 0;
	switch (type)
	{
	case 'b':
		val = o_document_value_byte(number);
		break;
	case 's':
		val = o_document_value_short(number);
		break;
	case 'i':
		val = o_document_value_int(number);
		break;
	case 'l':
		val = o_document_value_long(number);
		break;
	case 'f':
		val = o_document_value_float(number);
		break;
	case 'd':
		val = o_document_value_double(number);
		break;
	case 't':
		val = o_document_value_date(number);
		break;
	}
	return val;
}

struct o_document_value * o_doc_for_std_value_plain_deserialize(struct o_input_stream * stream)
{
	int readed = 0;
	struct o_document_value *value = 0;
	char exit = 0;
	struct o_string_buffer * buff = o_string_buffer_new();
	do
	{
		readed = o_input_stream_peek(stream);
		switch (readed)
		{
		case ')':
		case ']':
		case ',':
		case -1:
		{
			if (o_string_buffer_size(buff) > 0)
			{
				char * ca = o_string_buffer_str(buff);
				value = o_doc_for_std_value_plain_factory(ca, 'i');
				o_free(ca);
			}
			o_string_buffer_free(buff);
			exit = 1;
		}
			break;
		case 'b':
		case 's':
		case 'i':
		case 'l':
		case 'f':
		case 'd':
		case 't':
		{
			if (o_string_buffer_size(buff) > 0)
			{
				char * ca = o_string_buffer_str(buff);
				value = o_doc_for_std_value_plain_factory(ca, readed);
				o_free(ca);
			}
			o_string_buffer_free(buff);
			o_input_stream_read(stream);
			exit = 1;
		}
			break;
		default:
			if ((readed < '0' || readed > '9') && readed != '.' && readed != '-')
			{
				o_string_buffer_free(buff);
				char message[50];
				sprintf(message, "wrong number parsing '%c'", readed);
				throw(o_exception_new(message, 0));
			}
			o_string_buffer_append_char(buff, readed);
			o_input_stream_read(stream);
			break;
		}

	} while (exit == 0);
	return value;
}

struct o_document_value * o_document_value_map_deserialize(struct o_input_stream * stream)
{
	struct o_document_value *value = 0;
	int readed;
	do
	{
		readed = o_input_stream_read(stream);
	} while (readed != '}');
	return value;
}

struct o_document_value * o_doc_for_std_value_deserialize(struct o_input_stream * stream)
{
	struct o_document_value * value = 0;
	int readed = o_input_stream_peek(stream);
	switch (readed)
	{
	case '(':
	case '*':
		o_input_stream_read(stream);
		value = o_doc_for_std_value_embedded_deserialize(stream);
		break;
	case ')':
	case ']':
	case ',':
		break;
	case '"':
		o_input_stream_read(stream);
		value = o_doc_for_std_value_string_deserialize(stream);
		break;
	case '#':
		o_input_stream_read(stream);
		value = o_doc_for_std_value_link_deserialize(stream);
		break;
	case '[':
		o_input_stream_read(stream);
		value = o_doc_for_std_value_array_deserialize(stream);
		break;
	case '{':
		return o_document_value_map_deserialize(stream);
		break;
	case 't':
	case 'f':
		value = o_doc_for_std_value_bool_deserialize(stream);
		break;
	default:
		value = o_doc_for_std_value_plain_deserialize(stream);
		break;
	}
	if (o_input_stream_peek(stream) == ',')
		o_input_stream_read(stream);
	return value;
}

void o_document_formatter_standard_deserialize(struct o_document_formatter * fm, struct o_document * doc, struct o_input_stream * input)
{
	struct o_string_buffer * buff = o_string_buffer_new();
	int isfirst = 1;
	int readed;
	do
	{
		readed = o_input_stream_read(input);
		if (isfirst && readed == '@')
		{
			char * str = o_string_buffer_str(buff);
			o_document_set_class_by_name(doc, str);
			o_free(str);
			o_string_buffer_clear(buff);
		}
		else if (readed == ':')
		{
			isfirst = 0;
			char * str = o_string_buffer_str(buff);
			struct o_document_value * val = o_doc_for_std_value_deserialize(input);
			o_document_field_set(doc, str, val);
			o_free(str);
			o_string_buffer_clear(buff);
		}
		else if (readed != -1 && readed != ')' && readed != '*' && readed != ']' && readed != ',')
		{
			o_string_buffer_append_char(buff, readed);
		}
	} while (readed != -1 && readed != ')' && readed != '*');
	o_string_buffer_free(buff);
}

void o_document_formatter_standard_free_default(struct o_document_formatter * fm)
{
}

static struct o_document_formatter_standard default_form =
{ .formatter.o_document_formatter_deserialize = o_document_formatter_standard_deserialize, .formatter.o_document_formatter_serialize =
		o_document_formatter_standard_serialize, .formatter.o_document_formatter_free = o_document_formatter_standard_free_default };

struct o_document_formatter * o_document_formatter_standard_default()
{
	return (struct o_document_formatter *) &default_form;
}

struct o_document_formatter * o_document_formatter_standard_new()
{
	struct o_document_formatter_standard * form = o_malloc(sizeof(struct o_document_formatter_standard));
	form->formatter.o_document_formatter_deserialize = o_document_formatter_standard_deserialize;
	form->formatter.o_document_formatter_serialize = o_document_formatter_standard_serialize;
	form->formatter.o_document_formatter_free = o_document_formatter_standard_free;
	return &form->formatter;
}
