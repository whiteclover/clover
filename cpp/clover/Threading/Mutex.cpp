#include "Mutex.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


Mutex::Mutex()
{
	int rc;
	if ((rc = pthread_mutex_init(&_mutex, NULL)) != 0)
	{
		printf("Create Mutex failed: %d\n", rc);
		abort();
	}
}

Mutex::~Mutex()
{
	int rc;
	if ((rc = pthread_mutex_destroy(&_mutex)) != 0)
	{
		printf("Destory Mutex failed: %d\n", rc);
		abort();
	}
}

int Mutex::Lock()
{
	int rc;
	if ((rc = pthread_mutex_lock(&_mutex)) != 0)
	{
		printf("Mutex::Lock failed: %d\n", rc);
		abort();
		return -1;
	}
	return 0;
}

int Mutex::TryLock()
{
	int rc;
	if ( (rc = pthread_mutex_trylock(&_mutex)) != 0)
	{
		if (rc == EBUSY)
		{
			return 1;
		}
		else
		{
			printf("Mutex::TryLock failed: %d", rc);
			abort();
			return -1;
		}
	}
	return 0;
}


int Mutex::Unlock()
{
	int rc;
	if ((rc = pthread_mutex_unlock(&_mutex)) != 0)
	{
		printf("Mutex::Unlock failed: %d\n", rc);
		abort();
		return -1;
	}
	return 0;
}
