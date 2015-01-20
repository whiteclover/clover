#pragma once


#include "Mutex.h"
#include "Singleton.h"
#include "TcpConn.h"
#include "INetHandler.h"

class ConnectionPool
{
    friend class Singleton< ConnectionPool >;
    typedef  std::vector< TcpConn* >  Connections;
    
public:

    ConnectionPool() :m_used(0), m_size(0), m_pNetHandler(NULL) { }
    ~ConnectionPool() { ReleasePool(); }

    void Initialize(INetHandler * pNetHandler, int max_size = 1024*16);

    TcpConn* GetFreeConnection();
    int RestoreConnection(TcpConn* con);
    void ReleasePool();

private:
    
    int                 m_used;
    int                 m_size;
    int                 m_maxSize;
    Connections         m_connections;
    INetHandler*        m_pNetHandler;
    Mutex               m_lock;
};


#define sConnectionPool  Singleton< ConnectionPool >::Instance()