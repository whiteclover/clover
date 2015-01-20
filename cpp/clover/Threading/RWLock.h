#pragma once

#include "Mutex.h"

class RWLock
{
public:
    inline void AcquireReadLock()
    {
        _lock.Lock();
    }

    inline void ReleaseReadLock()
    {
        _lock.Unlock();
    }

    inline void AcquireWriteLock()
    {
        _lock.Lock();
    }

    inline void ReleaseWriteLock()
    {
        _lock.Unlock();
    }

private:
    Mutex _lock;
};