#pragma once

#include "Mutex.h"
#include <deque>

template<class TYPE>
class LockedQueue
{
public:

	~LockedQueue()
	{
	}

	inline void Enqueue(const TYPE & element)
	{
		_mutex.Lock();
		_queue.push_back(element);
		_mutex.Unlock();
	}


	inline TYPE Dequeue()
	{
		_mutex.Lock();
		TYPE t;
		if(_queue.size() == 0)
		{
			t = reinterpret_cast<TYPE>(0);
		}
		else
		{
			t = _queue.front();	
		}
		_mutex.Unlock();
		return t;
	}


	inline size_t Size()
	{
		_mutex.Lock();
		size_t c = _queue.size();
		_mutex.Unlock();
		return c;
	}


	inline void Clear()
	{
		_mutex.Lock();
		_queue.resize(0);
		_mutex.Unlock();
	}

protected:

	std::deque<TYPE> 	_queue;
	Mutex 				_mutex;
};

