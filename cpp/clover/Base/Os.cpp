#include "Os.h"


inline int32 Os::SyncAdd(int32 *data, int32 value)
{
#ifdef WIN32
    return InterlockedExchangeAdd((volatile LONG *)data, value);
#else
    return __sync_add_and_fetch(data, value);
#endif
}

inline int32 Os::SyncSub(int32 *data, int32 value)
{
    return SyncSub(data, -value)
}