#pragma once

#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <exception>
#include <google/protobuf/message.h>

#include "Common.h"

class ByteBufferException
{
public:
    
    ByteBufferException() {}
    virtual ~ByteBufferException() {}
};


class ByteBuffer
{
public:

    const static size_t DEFAULT_SIZE = 0x1000;

    ByteBuffer():_rpos(0), _wpos(0)
    {
        // allocate DEFAULT_SIZE
        _storage.reserve(DEFAULT_SIZE);
    }

    ByteBuffer(size_t res): _rpos(0), _wpos(0)
    {
        _storage.reserve(res);
    }

    ByteBuffer(const ByteBuffer &buf)
        :_rpos(buf._rpos),
        _wpos(buf._wpos), 
        _storage(buf._storage)
    {
    }

    virtual ~ByteBuffer() {}

    void Clear()
    {
        _storage.clear();
        _rpos = _wpos = 0;
    }


    template <typename T> void Append(T value)
    {
        Append((uint8*)&value, sizeof(value));
    }
        
    template <typename T> void Put(size_t pos, T value)
    {
        Put(pos, (uint8*)&value, sizeof(value));
    }

    // stream like operators for storing data
    ByteBuffer & operator<<(bool value) { Append<char>((char)value); return *this;}

    ByteBuffer & operator<<(uint8 value) { Append<uint8>(value); return *this; }
    ByteBuffer & operator<<(uint16 value) { Append<uint16>(value); return *this; }
    ByteBuffer & operator<<(uint32 value) { Append<uint32>(value); return *this; }
    ByteBuffer & operator<<(uint64 value) { Append<uint64>(value); return *this; }

    ByteBuffer & operator<<(int8 value) { Append<int8>(value); return *this; }
    ByteBuffer & operator<<(int16 value) { Append<int16>(value); return *this; }
    ByteBuffer & operator<<(int32 value) { Append<int32>(value); return *this;}
    ByteBuffer & operator<<(int64 value) { Append<int64>(value); return *this;}

    ByteBuffer & operator<<(float value) { Append<float>(value); return *this; }

    ByteBuffer & operator<<(double value) { Append<double>(value); return *this; }

    ByteBuffer & operator<<(const std::string & value) 
    {
        Append((uint8*)value.c_str(), value.length());
        Append((uint8)0);
        return *this;
    }
    
    ByteBuffer & operator<<(const char* str)
    {
        Append((uint8*)str, strlen(str));
        Append((uint8)0);
        return *this;
    }

    // stream like operators for Reading data
    ByteBuffer & operator>>(bool & value) { value = Read<char>() > 0 ? true : false; return *this;}

    ByteBuffer & operator>>(uint8 & value) { value = Read<uint8>(); return *this; }
    ByteBuffer & operator>>(uint16 & value) { value = Read<uint16>(); return *this; }
    ByteBuffer & operator>>(uint32 & value) { value = Read<uint32>(); return *this; }
    ByteBuffer & operator>>(uint64 & value) { value = Read<uint64>(); return *this; }

    ByteBuffer & operator>>(int8 & value) { value = Read<int8>(); return *this; }
    ByteBuffer & operator>>(int16 & value) { value = Read<int16>(); return *this; }
    ByteBuffer & operator>>(int32 & value) { value = Read<int32>(); return *this; }
    ByteBuffer & operator>>(int64 & value) { value = Read<int64>(); return *this; }

    ByteBuffer & operator>>(float & value) { value = Read<float>(); return *this; }

    ByteBuffer & operator>>(double & value) { value = Read<double>(); return *this; }
        
    ByteBuffer & operator>>(std::string & value)
    {
        value.clear();
        while(true)
        {
            char c = Read<char>();
            if(c == 0)
                break;
            value += c;
        }
        return *this;
    }

    uint8 operator[](size_t pos) { return Read<uint8>(pos); }

    size_t RPos() const { return _rpos; } 
    size_t RPos(size_t rpos) { _rpos = rpos; return _rpos; }

    size_t WPos() const { return _wpos; }
    size_t WPos(size_t wpos) { _wpos = wpos; return _wpos; }


    template <typename T> T Read()
    {
        T r = Read<T>(_rpos);
        _rpos += sizeof(T);
        return r;
    }
    
    template <typename T> T Read(size_t pos) const
    {
        //assert(pos + sizeof(T) <= Size());
        if(pos + sizeof(T) > Size())
        {
            printf("TTTTT\n");
            return (T)0;
        }
        else
        {
            return *((T*)&_storage[pos]);
        }
    }

    void Read(uint8* dest, size_t len)
    {
        if(LIKELY(_rpos + len <= Size()))
        {
            memcpy(dest, &_storage[_rpos], len);
        }
        else
        {
            //throw error();
            memset(dest, 0, len);
        }
        _rpos += len;
    }

    const uint8* Contents() const { return &_storage[0]; };
    uint8*  RawData() { return &_storage[0];};
    inline size_t Size() const { return _storage.size(); };
    
    // one should never use resize probably
    void Resize(size_t newsize)
    {
        _storage.resize(newsize);
        _rpos = 0;
        _wpos = Size();
    }
    
    inline void Reserve(size_t ressize)
    {
        if(ressize > Size()) _storage.reserve(ressize);
    }

    // Appending to the end of buffer
    void Append(const std::string & str)
    {
        Append((uint8*)str.c_str(), str.size() + 1);
    }
    
    void Append(const char* src, size_t cnt)
    {
        return Append((const uint8*)src, cnt);
    }
    
    void Append(const uint8* src, size_t cnt)
    {
        if(!cnt) return;

        // noone should even need uint8buffer longer than 10mb
        // if you DO need, think about it
        // then think some more
        // then use something else
        // -- qz
        // assert(size() < 10000000);


        // this way hopefully people will report the callstack after it "crashes"
        assert(Size() < 10000000);

        if(_storage.size() < _wpos + cnt)
            _storage.resize(_wpos + cnt);
        memcpy(&_storage[_wpos], src, cnt);
         _wpos += cnt;
    }
    
    void Append(const ByteBuffer & buffer)
    {
        if(buffer.Size() > 0) Append(buffer.Contents(), buffer.Size());
    }

    void Put(size_t pos, const uint8* src, size_t cnt)
    {
        assert(pos + cnt <= Size());
        memcpy(&_storage[pos], src, cnt);
    }
    
    //void insert(size_t pos, const uint8 *src, size_t cnt) {
    //  std::copy(src, src + cnt, inserter(_storage, _storage.begin() + pos));
    //}

    ByteBuffer & operator<<(const ::google::protobuf::Message & message);
    ByteBuffer & operator>>(::google::protobuf::Message & message);

    void AppendPackGUID(uint64 guid);
    uint64 UnpackGUID();
    
    void HexDump();

protected:
   
    size_t _rpos, _wpos;  // Read and write positions
    std::vector<uint8> _storage;
};


template <typename T> ByteBuffer & operator<<(ByteBuffer & b, std::vector<T> v)
{
    b << (uint32)v.size();
    for(typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer & operator>>(ByteBuffer & b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T> ByteBuffer & operator<<(ByteBuffer & b, std::list<T> v)
{
    b << (uint32)v.size();
    for(typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer & operator>>(ByteBuffer & b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V> ByteBuffer & operator<<(ByteBuffer & b, std::map<K, V> &m)
{
    b << (uint32)m.size();
    for(typename std::map<K, V>::iterator i = m.begin(); i != m.end(); i++)
    {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V> ByteBuffer & operator>>(ByteBuffer & b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while(msize--)
    {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}