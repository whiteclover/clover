#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <memory>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>
#include <arpa/inet.h>

#include "Common.h"

class ITcpConn
{
public:

    ITcpConn();

    virtual ~ITcpConn();
    
    virtual void Start(struct event_base * base, int  fd, struct sockaddr *address, int addrlen);

    virtual void Prepare() = 0;
    virtual void Finalize() = 0;

    void EnableEvent(int event);
    void DisableEvent(int event);

    void Send(const uint8* data, int size);
    int Index() const {return m_index; }
    void Index(int i) { m_index = i; }
    
    virtual void OnRead(struct bufferevent* bev) = 0;
    virtual void OnWrite(struct bufferevent* bev) = 0;
    virtual void OnEvent(struct bufferevent* bev, short event) = 0;

private:
    
    static void on_read_cb(struct bufferevent* bev, void* data);
    static void on_write_cb(struct bufferevent* bev, void* data);
    static void on_event_cb(struct bufferevent* bev, short event, void* data);


protected:

    struct bufferevent *                m_bev;
    struct event_base *                 m_evbase;
    int                                 m_index;
    int                                 m_sockfd;
    struct sockaddr_in                  m_sockaddr;
    socklen_t                           m_addrlen;
};


