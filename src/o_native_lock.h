#ifndef O_NATIVE_LOCK_H_
#define O_NATIVE_LOCK_H_

struct o_native_lock;

/*! \brief Create a new native lock.
 *
 * \return the new instance of lock.
 */
struct o_native_lock * o_native_lock_new();

/*! \brief lock a native lock.
 *
 * \param lock to lock.
 */
void o_native_lock_lock(struct o_native_lock * lock);

/*! \brief unlock a native lock.
 *
 * \param lock to unlock.
 */
void o_native_lock_unlock(struct o_native_lock * lock);

/*! \brief Free a native lock.
 *
 * \param lock to free.
 */
void o_native_lock_free(struct o_native_lock * lock);

struct o_native_cond;

/*! \brief create a new native condition.
 *
 * \param return a native lock condition.
 */
struct o_native_cond * o_native_cond_new();

/*! \brief wait the current thread on the condition.
 *
 * \param cond where wait.
 * \param lock context lock.
 */
void o_native_cond_wait(struct o_native_cond * cond, struct o_native_lock * lock);

/*! \brief notify a thread waiting on variable to unlock.
 *
 * \param cond to notify.
 */
void o_native_cond_notify(struct o_native_cond * cond);

/*! \brief Notify all thread waiting on variable.
 *
 * \param cond to notify.
 */
void o_native_cond_broadcast(struct o_native_cond * cond);

/*! \brief Free a condition.
 *
 * \param cond to free.
 */
void o_native_cond_free(struct o_native_cond * cond);

#endif /* O_NATIVE_LOCK_H_ */
