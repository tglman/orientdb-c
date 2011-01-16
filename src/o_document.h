#ifndef O_DOCUMENT_H_
#define O_DOCUMENT_H_
#include "o_record.h"

#define DOCUMENT_RECORD_TYPE 'd'

struct o_document_value;

/*! \brief A document.
 *
 */
struct o_document;

/**
 * instantiate a new document.
 *
 * @return new document instance.
 */
struct o_document * o_document_new();

/*! \brief Create a new document with id
 *
 * \param rid the record id.
 */
struct o_document * o_document_new_id(struct o_record_id * rid);

/**
 * Cast a document to record.
 *
 * @param doc document to cast.
 * @return the o_record casted.
 */
struct o_record * o_document_o_record(struct o_document * doc);

/**
 * Retrieve a value of an field from a doc.
 *
 * @param doc the from retrieve the value.
 * @param field_name the name of field.
 * @return the value of field.
 */
struct o_document_value * o_document_field_get(struct o_document * doc, char * field_name);

/**
 * set a value of a field in a doc.
 *
 * @param doc the document where set the value.
 * @param field_name the name where set value.
 * @param value the value to set.
 *
 */
void o_document_field_set(struct o_document * doc, char * field_name, struct o_document_value* value);

/** list the names of fields of document.
 *
 * @param doc the document from retrieve names.
 * @param names_count the variable where are put the number of names.
 * @return an array of names.
 *
 */
char ** o_document_field_names(struct o_document * doc, int *names_count);

/** list values of all fields.
 *
 * @param doc the doc from retrieve values.
 * @param values_count the variable where are put the number of values.
 * @return the array of values.
 */
struct o_document_value ** o_document_field_values(struct o_document * doc, int *values_count);

/** check if the document contains a field.
 *
 * @param doc where check if exist field.
 * @param filed_name the name of field to check.
 * @return 0 if field not exist otherwise a number != 0
 */
int o_document_contains_field(struct o_document *doc, char * field_name);

/** remove a field from a document.
 *
 * @param doc the document where remove field.
 * @param field_name the name of field to remove.
 */
void o_document_remove_field(struct o_document *doc, char * field_name);

/** create a copy of document.
 *
 * @param doc to copy.
 * @return a new copy of document.
 */
struct o_document * o_document_copy(struct o_document * doc);

/*! \brief write the document to buffer.
 *
 * \param doc to write.
 * \param buff where write.
 */
void o_document_serialize(struct o_document * doc, struct o_output_stream * output);

/*! \brief Read an document from the stream.
 *
 * \param doc to fill.
 * \param stream to read.
 */
void o_document_deserialize(struct o_document * doc, struct o_input_stream * stream);

/*! \brief Retreive the document class name.
 *
 * \param doc where retrieve the name.
 * \return the name of class of document.
 */
char * o_document_get_class_name(struct o_document * doc);

/** release the reference to document.
 *
 * @param doc to release.
 */
void o_document_release(struct o_document * doc);

/*! \brief increment an reference counter to document.
 *
 * \param doc to reverence
 */
void o_document_refer(struct o_document * doc);

#endif /* O_DOCUMENT_H_ */

