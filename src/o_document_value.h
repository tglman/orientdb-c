#ifndef O_DOCUMENT_VALUE_H_
#define O_DOCUMENT_VALUE_H_
#include "o_record.h"
#include "o_input_stream.h"

struct o_document;

enum o_document_value_type
{
	BYTE = 17, SHORT = 2, INT = 1, LONG = 3, BOOL = 0, FLOAT = 4, DOUBLE = 5, DATE = 6, STRING = 7, EMBEDDED = 9, LINK = 13, ARRAY = 10
};
/*! \brief a document field value.
 *
 */
struct o_document_value;

/*! \brief Create a new char(byte) value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_byte(char val);

/*! \brief Create a new short value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_short(short val);

/*! \brief Create a new int value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_int(int val);

/*! \brief Create a new long value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_long(long val);

/*! \brief Create a new bool value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_bool(int val);

/*! \brief Create a new float value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_float(float val);

/*! \brief Create a new double value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_double(double val);

/*! \brief Create a new date value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_date(int val);

/*! \brief Create a new string value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_string(char * str);

/*! \brief Create a new document embedded value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_embedded(struct o_document * doc);

/*! \brief Create a new record link value.
 *
 * \param val value.
 * \return new document value.
 */
struct o_document_value * o_document_value_link(struct o_record * doc);

/*! \brief Create a new link ref value.
 *
 * \param id the to link id.
 * \return a new document value
 */
struct o_document_value * o_document_value_link_ref(struct o_record_id *id);

/*! \brief Create a new array value.
 *
 * \param val value.
 * \param size the array size.
 * \return new document value.
 */
struct o_document_value * o_document_value_array(struct o_document_value * array[], int size);

/*! \brief Retrieve the value type.
 *
 * \param value where retrieve value.
 * \return the type of document value.
 */
enum o_document_value_type o_document_value_type(struct o_document_value * value);

/*! \brief Retrieve an byte value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
char o_document_value_get_byte(struct o_document_value * o_value);

/*! \brief Retrieve an short value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
short o_document_value_get_short(struct o_document_value * o_value);

/*! \brief Retrieve an int value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
int o_document_value_get_int(struct o_document_value * o_value);

/*! \brief Retrieve an long value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
long o_document_value_get_long(struct o_document_value * o_value);

/*! \brief Retrieve an bool value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
int o_document_value_get_bool(struct o_document_value * o_value);

/*! \brief Retrieve an float value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
float o_document_value_get_float(struct o_document_value * o_value);

/*! \brief Retrieve an double value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
double o_document_value_get_double(struct o_document_value * o_value);

/*! \brief Retrieve an date value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
int o_document_value_get_date(struct o_document_value * o_value);

/*! \brief Retrieve an string value from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
char * o_document_value_get_string(struct o_document_value * o_value);

/*! \brief Retrieve an document embedded from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
struct o_document * o_document_value_get_embedded(struct o_document_value * o_value);

/*! \brief Retrieve an document link from a document value.
 *
 * \param o_value the document value where retrieve value.
 */
struct o_record * o_document_value_get_link(struct o_document_value * o_value);

/*! \brief Retrieve the id of referred record.
 *
 * \param value that contains the id.
 * \return the id of referred record.
 */
struct o_record_id * o_document_value_get_link_ref(struct o_document_value * o_value);

/*! \brief Retrieve an array from a document value.
 *
 * \param o_value the document value where retrieve value.
 * \param array_size filled with the size of array
 */
struct o_document_value ** o_document_value_get_array(struct o_document_value * o_value, int *array_size);

/*! \Retrieve the size of array value.
 *
 * \param o_value array value.
 * \return the size of array.
 */
int o_document_value_get_array_size(struct o_document_value * o_value);

/*! \brief free the used memory of document value.
 *
 * \param to_free value to free.
 */
void o_document_value_free(struct o_document_value * to_free);

#endif /* O_DOCUMENT_VALUE_H_ */
