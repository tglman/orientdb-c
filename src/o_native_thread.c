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
};

struct o_native_thread * o_native_thread_new(char * name, void *(*function)(void *))
{
	struct o_native_thread * th = o_malloc(sizeof(struct o_native_thread));
	memset(th, sizeof(struct o_native_thread), 0);
	th->name = name;
	th->function = function;
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
    usleep(time*1000);
}

void o_native_thread_current_exit()
{
	pthread_exit(0);
}

