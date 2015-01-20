#pragma once

#include "Common.h"

#include <pthread.h>

class Condition
{

public:
	Condition();
	~Condition();

	int Acquire();
	int Release();

	int Signal();
	int Wait();
	int TimeWait(unsigned long time_ms);
	int Broadcast();

private:

	pthread_cond_t 	_cond;
	pthread_mutex_t _mutex;
};