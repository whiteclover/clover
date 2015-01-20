#include "ConnectionPool.h"


void ConnectionPool::Initialize(INetHandler * pNetHandler, int max_size = 1024*16) 
{   
    m_pNetHandler = pNetHandler;
    m_maxSize = (max_size < 1024)? 1024 : max_size; 
}

TcpConn* ConnectionPool::GetFreeConnection()
{
    ScopeMutex lock(m_lock);
    if (m_used == m_maxSize)
    {
        return NULL;
    }

    if (m_size == 0)
    {
        m_size = 128;
        m_connections.reserve(m_size);
        for (int i = 0; i < m_size; i++)
        {
            m_connections.push_back(TcpConn::Create(m_pNetHandler)) ;
        }
    }
    else if (m_used == m_size && m_used < m_maxSize)
    {
        m_size += 128;
        m_size = (m_size < m_maxSize) ? m_size : m_maxSize;
        m_connections.reserve(m_size);
        for (int i = m_used; i < m_size; i++)
        {
           m_connections.push_back(TcpConn::Create(m_pNetHandler)) ;
        }
    }
    m_connections[m_used]->Index(m_used);
    return m_connections[m_used++];
}

int ConnectionPool::RestoreConnection(TcpConn* con) 
{
    if (con == NULL) 
    {
        return -1;
    }

    if (-1 == con->Index())
    {
        return -1;
    }

    /* not last element */
    int i = con->Index();
    ScopeMutex lock(m_lock);
    if (i != m_used - 1) 
    {
        TcpConn* temp;
        temp = m_connections[i];
        m_connections[i] = m_connections[m_used - 1];
        m_connections[m_used - 1] = temp;

        m_connections[i]->Index(i);
        m_connections[m_used - 1]->Index(-1);
    }
    m_used--;
    con->Index(-1);
    return 0;
}

void ConnectionPool::ReleasePool()
{
    ScopeMutex lock(m_lock);
    m_connections.clear();
    m_used = 0;
    m_size = 0;
}