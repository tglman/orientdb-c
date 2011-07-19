#ifndef O_MAP_H_
#define O_MAP_H_

/**
 * The internal map.
 */
struct o_map;

/**
 *  The map constructor.
 *
 *  @return a new map instance.
 */
struct o_map * o_map_new(unsigned int(*o_map_hash)(void *), void(*o_entry_create)(void **, void **), void(*o_entry_free)(void **, void **),
		int(*o_key_compare)(void * key1, void * key2));

/**
 * put a value into map.
 *
 * @param map target where put the value.
 * @param key the key of association.
 * @param value the value of association.
 * @return the old value associate to key or 0 if not exist an association.
 */
void * o_map_put(struct o_map * map, void * key, void * val);

/** Remove a value from the map;
 *
 * @param map target where get the value.
 * @param key the key of association.
 * @return the value of association or 0 if not exist.
 */
void * o_map_remove(struct o_map * map, void * key);

/** Retrieve a value from the map if not exist an association return 0;
 *
 * @param map target where get the value.
 * @param key the key of association.
 * @return the value of association or 0 if not exist.
 */
void * o_map_get(struct o_map * map, void * key);

/** Retrieve an array of keys.
 *
 * @param map the map to iterate keys.
 * @param keys_num the int pointer fill with the number of keys.
 * @return an array of keys.
 */
void ** o_map_keys(struct o_map * map, int * keys_num);

/** Retrieve an array of values from the map.
 *
 * @param map the map to iterate values.
 * @param values_num the numbers of values retrieved.
 * @return an array of values.
 */
void ** o_map_values(struct o_map * map, int * values_num);

/** Retrieve the number of elements in map.
 *
 * @param map where retrieve the size.
 * @return the size of map.
 */
int o_map_size(struct o_map * map);

/*! \brief remove all entries from the map.
 *
 * \param map to clear.
 */
void o_map_clear(struct o_map * map);

/*! \brief Retrieve the first key of map.
 *
 * \param map where retrieve.
 * \return the first key.
 */
void * o_map_first_key(struct o_map * map);

/*! \brief Retrieve the last key of map.
 *
 * \param map where retrieve.
 * \return the first key.
 */
void * o_map_last_key(struct o_map * map);

/** Free the structures of map.
 *
 * @param map structures to free.
 */
void o_map_free(struct o_map * map);

#endif /* O_MAP_H_ */
