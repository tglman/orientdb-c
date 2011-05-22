#ifndef O_POOL_H_
#define O_POOL_H_

struct o_pool;

/*! \brief Create a new Pool
 *
 * \param obj object factory.
 * \param factory function used to create new instance of objects.
 * \param free function used to release instance of objects.
 * \return the new pool instance.
 */
struct o_pool * o_pool_new(void * obj, void * (*factory)(void *), void(*free)(void *, void *));

/*! \brief Create a new Pool
 *
 * \param obj object factory.
 * \param factory function used to create new instance of objects.
 * \param free function used to release instance of objects.
 * \param min_size minus pool size.
 * \param max_size max pool size.
 * \return the new pool instance.
 */
struct o_pool * o_pool_new_size(void * obj, void * (*factory)(void *), void(*factory_free)(void *, void *), int min_size, int max_size);

/*! \brief Retrieve an instance from pool or wait until an instance is free in pool.
 *
 * \param pool where retrieve the instance.
 * \return an instance.
 */
void * o_pool_get(struct o_pool * pool);

/*! \brief Release an instance to the pool.
 *
 * \param pool where release the instance.
 * @param to_release instance to release.
 */
void o_pool_release(struct o_pool * pool, void * to_release);

/*! \brief destroy  an instance of pool.
 *
 * \param to_free pool to free.
 */
void o_pool_free(struct o_pool * to_free);

#endif /* O_POOL_H_ */
