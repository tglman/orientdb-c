#ifndef O_MAP_STRING_H_
#define O_MAP_STRING_H_

/**
 * The internal String -> map.
 */
struct o_map_string;

/**
 *  The map constructor.
 *
 *  @return a new map instance.
 */
struct o_map_string * o_map_string_new();

/**
 * put a value into map.
 *
 * @param map target where put the value.
 * @param key the key of association.
 * @param value the value of association.
 * @return the old value associate to key or 0 if not exist an association.
 */
void * o_map_string_put(struct o_map_string * map, char * key, void * val);

/** Remove a value from the map;
 *
 * @param map target where get the value.
 * @param key the key of association.
 * @return the value of association or 0 if not exist.
 */
void * o_map_string_remove(struct o_map_string * map, char * key);

/** Retrieve a value from the map if not exist an association return 0;
 *
 * @param map target where get the value.
 * @param key the key of association.
 * @return the value of association or 0 if not exist.
 */
void * o_map_string_get(struct o_map_string * map, char * key);

/** Retrieve an array of keys.
 *
 * @param map the map to iterate keys.
 * @param keys_num the int pointer fill with the number of keys.
 * @return an array of keys.
 */
char ** o_map_string_keys(struct o_map_string * map, int * keys_num);

/** Retrieve an array of values from the map.
 *
 * @param map the map to iterate values.
 * @param values_num the numbers of values retrieved.
 * @return an array of values.
 */
void ** o_map_string_values(struct o_map_string * map, int * values_num);

/** Retrieve the number of elements in map.
 *
 * @param map where retrieve the size.
 * @return the size of map.
 */
int o_map_string_size(struct o_map_string * map);

/*! \brief Remove all entry from string map.
 *
 * \param map to clear.
 */
void o_map_string_clear(struct o_map_string * map);

/** Free the structures of map.
 *
 * @param map structures to free.
 */
void o_map_string_free(struct o_map_string * map);

#endif /* O_MAP_STRING_H_ */
