#ifndef O_DATABASE_INTERNAL_H_
#define O_DATABASE_INTERNAL_H_
#include "o_database.h"
#include "o_string_buffer.h"

struct o_database
{
	struct o_database_error_handler * error_handler;
	char * connection_url;
	struct o_storage * storage;
	void (*o_record_writer)(struct o_record *, struct o_string_buffer *);
};

/** Fill a new instance of a database and associate an error hendler
 *
 * @param db to fill.
 * @param connection_url the database connection url.
 * @param error_handler the handler of eventually errors.
 * @return the new database instance.
 */
void o_database_new_internal(struct o_database * db, char * connection_url, struct o_database_error_handler * error_handler);

/** Clear the database structure .
 *
 * @param db to clear.
 */
void o_database_free_internal(struct o_database * db);

#endif /* O_DATABASE_INTERNAL_H_ */
