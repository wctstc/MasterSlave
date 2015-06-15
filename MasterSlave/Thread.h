#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class Thread
{
public:
	static int CreateThread( pthread_t *thread_id, void *(*function) (void *), void *arg );
	static int KillThread( pthread_t *thread_id );
	static int JoinThread( pthread_t *thread_id );
};

#endif