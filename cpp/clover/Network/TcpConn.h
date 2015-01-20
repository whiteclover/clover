#pragma once


#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>
#include <arpa/inet.h>

#include "NetPacket.h"
#include "DoubleBuffer.h"
#include "ByteBuffer.h"
#include "INetHandler.h"
#include "ITcpConn.h"

class Session;

class TcpConn: public ITcpConn
{
public:
    
    static const int HEADER_SIZE = sizeof(uint16) *2; // Pack len & Opcode


    TcpConn();
    TcpConn(INetHandler * pNetHandler);
    ~TcpConn();
    
    void Reset();
    void Disconnect();
    
    int GetSessionId() const;
    void SetNetHander(INetHandler * INetHandler);

    virtual void Prepare() override;
    virtual void Finalize() override;
    virtual void OnRead(struct bufferevent * bev)override;
    virtual void OnWrite(struct bufferevent * bev)override;
    virtual void OnEvent(struct bufferevent * bev, short event)override;


    void Send(const NetPacket &pack);
    void Send(const ByteBuffer & buff);
    
    static TcpConn* Create(INetHandler* pHandler);

    void Push(const NetPacket& pack);
    void Push(const ByteBuffer& buf);
    void Flush();   
    int DealWithError();

protected:

    virtual void OnTimeOut();
    virtual void OnError();
    virtual void OnEOF();
    virtual void OnConnected();


private:
    Session *                           m_session;
    INetHandler*                        m_pHandler;
    std::auto_ptr<NetPacket>            m_pRecvPack;
    DoubleBuffer                        m_sendDbuf;
    uint16                              m_opcode;
    uint16                              m_len;
};