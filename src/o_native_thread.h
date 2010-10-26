#ifndef O_NATIVE_THREAD_H_
#define O_NATIVE_THREAD_H_

struct o_native_thread;

/*! \brief Create a new thread
 *
 * \param name the name of new thread.
 * \param function to execute
 * \return the new instance of thread.
 */
struct o_native_thread * o_native_thread_new(char * name, void *(*function)(void *));

/*! \brief start the execution of thread.
 *
 * \param thread to execute.
 */
void o_native_thread_start(struct o_native_thread * thread);

/*! \brief join to thread.
 *
 * \param thread to join.
 */
void o_native_thread_join(struct o_native_thread * thread);

/*! \brief sleap the current thread for millisec time.
 *
 * \param time of thread sleep.
 */
void o_native_thread_current_sleep(int time);

/*! \brief called at the end of current thread.
 *
 */
void o_native_thread_current_exit();

#endif /* O_NATIVE_THREAD_H_ */
