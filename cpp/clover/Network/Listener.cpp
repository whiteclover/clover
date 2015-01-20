#include "Listener.h"

#include <unistd.h>



std::string Listener::GetHost() const { return m_host;}
int Listener::GetPort() const { return m_port; }

int Listener::Start()
{
    m_ev_config = event_config_new();
    if (!m_ev_config) 
    {
        perror("Couldn't create config");
        return -1;
    }
    event_config_require_features(m_ev_config, EV_FEATURE_ET);
    event_config_set_flag(m_ev_config, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    m_base = event_base_new_with_config(m_ev_config);
    if (!m_base) 
    {
        perror("Couldn't create listener");
        return -1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    sin.sin_port = ntohs((u_short)m_port);
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if( m_host != "0.0.0.0")
    {
        struct hostent* hostname = gethostbyname(m_host.c_str());
        if(hostname != 0)
        {
            memcpy(&sin.sin_addr.s_addr, hostname->h_addr_list[0], hostname->h_length);
        }
    }

    m_listener = evconnlistener_new_bind(
            m_base, 
            accept_conn_cb, 
            this,
            LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, 
            -1,
            (struct sockaddr*)&sin, 
            sizeof(sin)
    );

    if (!m_listener) 
    {
        perror("Couldn't create listener");
        return -1;
    }

    evconnlistener_set_error_cb(m_listener, accept_error_cb);
    printf("Start servering\n");
    event_base_dispatch(m_base);
    return 0;
}

int Listener::Stop()
{
    event_base_loopbreak(m_base);
    if(m_listener)
    {
        evconnlistener_free(m_listener);
        m_listener = NULL;
    }   
    event_config_free(m_ev_config);
    if(m_base)
    {
        event_base_free(m_base);
        m_base = NULL;
    }
    return 0;
}

void Listener::accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
    Listener * l = static_cast<Listener *>(ctx);
    TcpConn*  con = sConnectionPool->GetFreeConnection();
    if (con)
    {
        con->Start(l->m_base, fd, address, socklen);
    }
    else 
    {
        // Todo: Implement Handler for connection pool is busy...
        close(fd);
    }
}


void Listener::accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
        "Shutting down.\n", err, evutil_socket_error_to_string(err));
}
