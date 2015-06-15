
#include <pthread.h>
#include "Thread.h"

int Thread::CreateThread(pthread_t *thread_id, void *(*function) (void *), void *arg)
{
	return pthread_create( thread_id, 0, function, arg );
}

int Thread::KillThread(pthread_t *thread_id)
{
	return pthread_kill( *thread_id, SIGILL );
}

int Thread::JoinThread(pthread_t *thread_id)
{
	return pthread_join( *thread_id, NULL );
}


