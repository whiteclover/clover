#pragma once

#include "NetPacket.h"
#include "TcpConn.h"

class Session
{
public:
    Session() {}
    virtual ~Session(){}

    inline void Send(const NetPacket & pack) { m_conn->Send(pack); }
    inline void Send(const ByteBuffer & buff) { m_conn->Send(buff); }

    inline void SetTcpConn(TcpConn * con) { m_conn = con; }
    inline TcpConn * GetTcpConn() const { return m_conn; }
    
    inline int SessionId() const { return m_conn->Index(); }
    inline void Reset() { m_conn->Reset(); };

private:

    TcpConn * m_conn;
};