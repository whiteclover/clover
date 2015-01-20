#pragma once


#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


#include "ConnectionPool.h"
#include "TcpConn.h"

class Listener
{
public:

    Listener() {};

    virtual ~Listener() {}
    int Initialize(const std::string& host, int port)
    {
        m_port = port;
        m_host = host;
        return 0;
    }
    
    std::string GetHost() const;
    int GetPort() const;

    int Start();
    int Stop();

    static void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);

    static void accept_error_cb(struct evconnlistener *listener, void *ctx);

private:

    int                     m_port;
    std::string             m_host;
    struct event_config*    m_ev_config;
    struct evconnlistener*  m_listener;
    struct event_base*      m_base;
};
