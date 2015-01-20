#pragma once

#include "Common.h"

template <class T>
class Singleton
{
    
public:

    static T* Instance() 
    {
        if(!_instacne) _instacne = new T;
        assert(_instacne != NULL);
        return _instacne;
    }

protected:

    Singleton();
    ~Singleton();

private:

    Singleton(Singleton const&);
    Singleton & operator=(Singleton const&);
    static T* _instacne;
};

template <class T> T* Singleton<T>::_instacne=NULL;