#include "o_native_thread.h"
#include "o_memory.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
struct o_native_thread
{
	char * name;
	void *(*function)(void *);
	pthread_t thread;
	int count;
};

struct o_native_thread * o_native_thread_new(char * name, void *(*function)(void *))
{
	struct o_native_thread * th = o_malloc(sizeof(struct o_native_thread));
	memset(th, 0, sizeof(struct o_native_thread));
	th->name = name;
	th->function = function;
	th->count = 1;
	return th;
}

void o_native_thread_start(struct o_native_thread * thread)
{
	pthread_create(&thread->thread, (pthread_attr_t *) 0, thread->function, 0);
}

void o_native_thread_join(struct o_native_thread * thread)
{
	pthread_join(thread->thread, 0);
}

void o_native_thread_current_sleep(int time)
{
	usleep(time * 1000);
}

void o_native_thread_release(struct o_native_thread * to_release)
{
	to_release->count--;
	if (to_release->count == 0)
	{
		o_free(to_release);
	}
}

void o_native_thread_current_exit()
{
	pthread_exit(0);
}

