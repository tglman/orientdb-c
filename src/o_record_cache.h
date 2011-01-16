#ifndef O_RECORD_CACHE_H_
#define O_RECORD_CACHE_H_
#include "o_record.h"

struct o_record_cache;

/*! \brief Create a cache on record.
 *
 * \return the new cache instane.
 */
struct o_record_cache * o_record_cache_new();

/*! \brief put a record in the cache.
 *
 * \param cache where put.
 * \param record to put in cache.
 */
void o_record_cache_put(struct o_record_cache * cache, struct o_record * record);

/*! \brief search a record from the cache with the record id.
 *
 * \param cache where search.
 * \param id to search.
 * \return the instance of record otherwise 0;
 */
struct o_record * o_record_cache_get(struct o_record_cache * cache, struct o_record_id *id);

/*! \brief Remove a record from the cache .
 *
 * \param cache where remove.
 * \param record to remove.
 */
void o_record_cache_remove(struct o_record_cache * cache, struct o_record * record);

/*! \brief remove all record from cache.
 *
 * \param cache to clear.
 */
void o_record_cache_clear(struct o_record_cache * cache);

/*! \brief free the cache structure.
 *
 * \param cache to free.
 */
void o_record_cache_free(struct o_record_cache * cache);

#endif /* O_RECORD_CACHE_H_ */
