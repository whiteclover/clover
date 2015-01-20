#include "Thread.h"


void * thread_proc(void *data)
{
	Thread * t = static_cast< Thread * >(data);
	t->_Run();
	return NULL;
}

int Thread::Start()
{
	if (_is_running)
	{
		return 0;
	}

	if (Initialize() < 0)
	{
		return -1;
	}

	_is_running  = true;
	if (pthread_create(&_thread, NULL, thread_proc, this) != 0)
	{
		return -1;
	}

	return 0;
}

int Thread::Stop()
{
	if (!_is_running)
	{
		return 0;
	}

	_is_running = false;

	void* res = NULL;
	if (pthread_join(_thread, &res) != 0)
	{
		return -1;
	}

	Finalize();
	
	return 0;
}

void Thread::_Run()
{
	while(_is_running)
	{
		Execute();
	}
}