#include "o_native_lock.h"
#include "o_memory.h"
#include <pthread.h>

struct o_native_lock
{
	pthread_mutex_t mutex;
};

struct o_native_cond
{
	pthread_cond_t cond;
};

struct o_native_lock * o_native_lock_new()
{
	struct o_native_lock * lock = o_malloc(sizeof(struct o_native_lock));
	pthread_mutex_init(&lock->mutex, 0);
	return lock;
}

void o_native_lock_lock(struct o_native_lock * lock)
{
	pthread_mutex_lock(&lock->mutex);
}

void o_native_lock_unlock(struct o_native_lock * lock)
{
	pthread_mutex_unlock(&lock->mutex);
}

void o_native_lock_free(struct o_native_lock * lock)
{
	pthread_mutex_destroy(&lock->mutex);
	o_free(lock);
}

struct o_native_cond * o_native_cond_new()
{
	struct o_native_cond * cond = o_malloc(sizeof(struct o_native_cond));
	pthread_cond_init(&cond->cond, 0);
	return cond;
}

void o_native_cond_wait(struct o_native_cond * cond, struct o_native_lock * lock)
{
	pthread_cond_wait(&cond->cond, &lock->mutex);
}

void o_native_cond_notify(struct o_native_cond * cond)
{
	pthread_cond_signal(&cond->cond);
}

void o_native_cond_broadcast(struct o_native_cond * cond)
{
	pthread_cond_broadcast(&cond->cond);
}

void o_native_cond_free(struct o_native_cond * cond)
{
	pthread_cond_destroy(&cond->cond);
	o_free(cond);
}
