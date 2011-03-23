#ifndef O_DATABASE_INTERNAL_H_
#define O_DATABASE_INTERNAL_H_
#include "o_record.h"
#include "o_database.h"
#include "o_string_buffer.h"
#include "o_record_cache.h"
#include "o_raw_buffer.h"
#include "o_list.h"
#include "o_query_callback.h"

struct o_database
{
	struct o_database_error_handler * error_handler;
	char * connection_url;
	struct o_storage * storage;
	struct o_record_cache * cache;
	struct o_list * referrers;
	char db_type;
};

/*! \brief Fill a new instance of a database and associate an error hendler
 *
 * \param db to fill.
 * \param connection_url the database connection url.
 * \param error_handler the handler of eventually errors.
 * \param db_type the type of instantiate database.
 * \return the new database instance.
 */
void o_database_new_internal(struct o_database * db, char * connection_url, struct o_database_error_handler * error_handler, char db_type);

/*! \brief Retrieve the cache from database.
 *
 * \param db where retrieve the cache.
 * \param the record cache of db.
 */
struct o_record_cache * o_database_get_cache(struct o_database * db);

/*! \brief Create a record form id and content, and cache it in the db cache.
 *
 * \param db where cache.
 * \param id the id of record to create.
 * \param content the content to fill the record.
 * \return the new record instance.
 */
struct o_record * o_database_record_from_content(struct o_database * db, struct o_record_id * rid, struct o_raw_buffer * content);

/*! \brief Add a referrer to the current db.
 *
 * \param db to refer.
 * \param refferrer poiter who refer.
 */
void o_database_add_referrer(struct o_database * db, struct o_database ** referrer);

/*! \brief Remove a referrer to current database.
 *
 * \param db to refer.
 * \param referrer to remove.
 */
void o_database_remove_referrer(struct o_database * db, struct o_database ** referrer);

/*! \brief Exucute a query on database and put result in the call back.
 *
 * \param db where execute query.
 * \param query to execute.
 * \param add_info gived to callback.
 * \param callback to invoke on result put.
 */
void o_database_query_internal(struct o_database * db, struct o_query * query, void *add_info, query_result_callback callback);

/** Clear the database structure .
 *
 * @param db to clear.
 */
void o_database_free_internal(struct o_database * db);

#endif /* O_DATABASE_INTERNAL_H_ */
