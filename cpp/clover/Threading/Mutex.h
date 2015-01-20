#pragma once

#include <pthread.h>

class Mutex
{

public:

    Mutex();
    virtual ~Mutex();

    int Lock();
    int TryLock();
    int Unlock();

protected:
    pthread_mutex_t _mutex;
};


class ScopeMutex 
{ 

public: 

    explicit ScopeMutex(Mutex& mutex) : _mutex(mutex) {  _mutex.Lock();  } 
    ~ScopeMutex() {  _mutex.Unlock();  } 

private: 

    // nocopyable
    ScopeMutex(const ScopeMutex&);
    ScopeMutex& operator=(const ScopeMutex&);

    Mutex& _mutex;   
};