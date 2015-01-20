#pragma once


#include <cstdlib>
#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <assert.h>

#ifdef __GNUC__
    typedef long long int64;
    typedef int int32;
    typedef short int16;
    typedef signed char int8;
    typedef unsigned long long uint64;
    typedef unsigned int uint32;
    typedef unsigned short uint16;
    typedef unsigned char uint8;
    typedef unsigned int DWORD;
    typedef unsigned char byte;
#else
    typedef signed __int64 int64;
    typedef signed __int32 int32;
    typedef signed __int16 int16;
    typedef signed __int8 int8;
    typedef unsigned __int64 uint64;
    typedef unsigned __int32 uint32;
    typedef unsigned __int16 uint16;
    typedef unsigned __int8 uint8;
#endif



//#include <iostream>

#if defined ( __GNUC__ )
#   define LIKELY( _x ) \
        __builtin_expect( ( _x ), 1 )
#   define UNLIKELY( _x ) \
        __builtin_expect( ( _x ), 0 )
#else
#   define LIKELY( _x ) \
        _x
#   define UNLIKELY( _x ) \
        _x
#endif
