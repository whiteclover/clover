#include "Condition.h"
#include <sys/time.h>

Condition::Condition()
{
    int rc;
    if ((rc = pthread_cond_init(&_cond, NULL)) != 0)
    {
        printf("pthread_cond_init: %d\n", rc);
    }

    if ((rc = pthread_mutex_init(&_mutex, NULL)) != 0)
    {
        if ((rc = pthread_cond_destroy(&_cond)) != 0)
        {
            printf("pthread_cond_destroy: %d\n", rc);

        }
    }
}


Condition::~Condition()
{
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond);
}


int Condition::Acquire()
{
    int rc;
    if ((rc = pthread_mutex_lock(&_mutex)) != 0)
    {
        printf("pthread_mutex_lock: %d\n", rc);
        return -1;
    }
    return 0;   
}

int Condition::Release()
{
    int rc;
    if ((rc = pthread_mutex_unlock(&_mutex)) != 0)
    {
        printf("pthread_mutex_unlock: %d\n", rc);
        return -1;
    }
    return 0;
}


int Condition::Signal()
{
    int rc;
    if ((rc = pthread_cond_signal(&_cond)) != 0)
    {
        printf("pthread_cond_signal: %d\n", rc);
        return -1;
    }

    return 0;
}

int Condition::TimeWait(unsigned long time_ms)
{
    unsigned long times = time_ms / 1000;
    time_ms = time_ms - times * 1000;

    timeval now;
    timespec tv;

    gettimeofday(&now, NULL);

    tv.tv_sec = now.tv_sec;
    tv.tv_nsec = now.tv_usec * 1000;
    tv.tv_sec += times;
    tv.tv_nsec += (time_ms * 1000 * 1000);
    int rc;
    if ((rc = pthread_cond_timedwait(&_cond, &_mutex, &tv)) != 0)
    {
        printf("pthread_cond_timedwait: %d\n", rc);
        return -1;
    }
    return 0;
}


int Condition::Wait()
{
    int rc;
    if ((rc = pthread_mutex_lock(&_mutex)) != 0)
    {
        printf("pthread_mutex_lock: %d\n", rc);
        return -1;
    }
    return 0;
}

int Condition::Broadcast()
{
    int rc;
    if ((rc = pthread_cond_broadcast(&_cond)) != 0)
    {
        printf("pthread_cond_broadcast: %d\n", rc);
        return -1;
    }
    return 0;
}