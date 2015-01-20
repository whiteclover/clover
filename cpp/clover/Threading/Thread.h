#pragma once

#include <pthread.h>
#include <stdlib.h>

class Thread 
{
    friend void * thread_proc(void * data);

public:
    
    Thread(): _is_running(false) {}
    virtual ~Thread() {};

    virtual int Start();
    virtual int Stop();
    
    pthread_t GetIdent() const {return _thread; }
    bool IsRunning() const {return _is_running; }


protected:

    virtual int Initialize() =0;
    virtual int Execute() = 0;
    virtual int Finalize() = 0;

    virtual void _Run();

    pthread_t _thread;
    volatile bool  _is_running;
};