#ifndef O_RECORD_CONTENT_H_
#define O_RECORD_CONTENT_H_

struct o_record_content;

/**
 * Create a new record content structure.
 * @return the new record content structure.
 */
struct o_record_content * o_record_content_new();

char* o_record_content_bytes(struct o_record_content * content);

int o_record_content_bytes_length(struct o_record_content * content);

char o_record_content_type(struct o_record_content * content);

/**
 * Free a record content structure.
 *
 * @param content to free.
 */
void o_record_content_free(struct o_record_content * content);

#endif /* O_RECORD_CONTENT_H_ */
