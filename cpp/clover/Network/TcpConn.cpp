#include "Session.h"
#include "TcpConn.h"

#include "ConnectionPool.h"

#include <unistd.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <stdio.h>
#include <errno.h>

TcpConn::TcpConn()
    :m_session(NULL)
    ,m_pRecvPack(NULL)
    ,m_pHandler(NULL)
{
    m_len= 0;
    m_opcode = 0;
}


TcpConn::TcpConn(INetHandler * pNetHandler)
    :m_session(NULL)
    ,m_pRecvPack(NULL)
    ,m_pHandler(pNetHandler)
{
    m_session = pNetHandler->CreateNewSession();
    m_session->SetTcpConn(this);
    m_len= 0;
    m_opcode = 0;
}

TcpConn::~TcpConn()
{
    Disconnect();
    if (m_session != NULL)
    {
        delete m_session;
    }
}


void TcpConn::Prepare()
{
    m_pHandler->OnConnect(m_session);
}

void TcpConn::Finalize()
{

}

void TcpConn::Reset()
{
    Disconnect();
    sConnectionPool->RestoreConnection(this);
}


void TcpConn::SetNetHander(INetHandler * pNetHandler)
{
    m_pHandler = pNetHandler;
    if (m_session != NULL)
    {
        delete m_session;
    }
    m_session = pNetHandler->CreateNewSession();
    m_session->SetTcpConn(this);
}

void TcpConn::Disconnect()
{
    if(m_bev != NULL)
    {
        bufferevent_free(m_bev);
        m_bev = NULL;
    }
    m_len= 0;
    m_opcode = 0;
    m_sendDbuf.OutBuff().Clear();
    m_sendDbuf.InBuff().Clear();
    close(m_sockfd);
    m_pHandler->OnDisconnect(m_session);
    m_sockfd = -1;
    m_evbase = NULL;
}

TcpConn* TcpConn::Create(INetHandler * pNetHandler)
{
    return new TcpConn(pNetHandler);
}

void TcpConn::Send(const NetPacket & pack)
{
    Push(pack);
    Flush();
}

void TcpConn::Send(const ByteBuffer & buff)
{
    Push(buff);
    Flush();
}

void TcpConn::Push(const NetPacket& pack)
{
    ByteBuffer& buf = m_sendDbuf.InBuff();
    pack.AppendTo(buf);
}

void TcpConn::Push(const ByteBuffer& buff )
{
    m_sendDbuf.InBuff().Append(buff);
}

void TcpConn::Flush()
{
    if (m_sendDbuf.OutBuff().Size() > 0)
    {
        return;
    }

    if (m_sendDbuf.InBuff().Size() == 0)
    {
        return;
    }
    m_sendDbuf.SwapInOut();
    ITcpConn::Send(m_sendDbuf.OutBuff().Contents(), (int)m_sendDbuf.OutBuff().Size());
}

void TcpConn::OnRead(struct bufferevent * bev)
{

    struct evbuffer * buf;
    buf = bufferevent_get_input(bev);
    uint64_t length = evbuffer_get_length(buf);
    printf("length:%ld\n", length );
    uint8 *data = NULL;


    while(true)
    {
        if(evbuffer_get_length(buf) < HEADER_SIZE)
        {
            return ;    
        } 

        if(!m_len)
        {
            if ( (data = evbuffer_pullup(buf, HEADER_SIZE)) != NULL )
            {
                evbuffer_remove(buf, data, HEADER_SIZE);
                m_opcode = *data;
                m_len = *(data+2);

                m_pRecvPack.reset(new NetPacket(m_opcode, m_len));
                m_pRecvPack->Resize(m_len);
                printf("Data : %d, opcode:%d, data Size: %d\n", data, m_opcode, m_len );
                //evbuffer_drain(buf, HEADER_SIZE);
                //free(data);
                data = NULL;
               
            }
            else
            {
                return ;
            }
        }


        if(m_pRecvPack->Size() > 0 &&  (data = evbuffer_pullup(buf, m_pRecvPack->Size())) != NULL)
        {
            evbuffer_remove(buf, m_pRecvPack->RawData(), m_pRecvPack->Size());
            //printf("Data: %s, Pack body size: %d\n", m_pRecvPack->RawData(), m_pRecvPack->Size());
            data = NULL;
            if (m_pHandler)
            {
               m_pHandler->OnRead(m_session, m_pRecvPack.release());
            }
        }

        m_len = 0;
        m_opcode = 0;

    }
}

void TcpConn::OnWrite(struct bufferevent* bev)
{
    m_sendDbuf.OutBuff().Clear();
    //EnableEvent(EV_WRITE);
}

void TcpConn::OnEvent(struct bufferevent * bev, short event)
{
  
    if(event & BEV_EVENT_READING)
    {
    }
    if(event & BEV_EVENT_WRITING)
    {
    }
    if(event & BEV_EVENT_ERROR)
    {
        printf("got BEV_EVENT_ERROR.errcode:%d, error:%s\n", EVUTIL_SOCKET_ERROR(), 
            evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
        OnError();
    }
    if(event & BEV_EVENT_TIMEOUT)
    {
        printf("got BEV_EVENT_TIMEOUT\n");
        OnTimeOut();
    }
    if(event & BEV_EVENT_EOF)
    {
        printf("got BEV_EVENT_EOF\n");
        OnEOF();
    }
    if(event & BEV_EVENT_CONNECTED)
    {
        OnConnected();
    }
    
}


void TcpConn::OnTimeOut()
{
    Reset();
    DealWithError();
}

void TcpConn::OnError()
{
    Reset();
    DealWithError();
}

void TcpConn::OnEOF()
{
    Reset();
}

void TcpConn::OnConnected()
{

}

int TcpConn::DealWithError()
{
    return 0;
}
