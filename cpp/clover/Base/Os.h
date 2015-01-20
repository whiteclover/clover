#pragma once

#include "Common.h"
#include "Singleton.h"

class Os
{
    friend class SingletonT< Os >;

    private:
        Os() {} 
        virtual ~Os() {}

    public:
       inline int32 SyncAdd(int32 *data, int32 value);
       inline int32 SyncSub(int32 *data, int32 value);
};


#define sOs Singleton< Os >::Instance()

