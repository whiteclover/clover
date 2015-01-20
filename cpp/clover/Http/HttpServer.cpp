#include "HttpServer.h"
#include "WebApplication.h"

HttpServer::HttpServer(const std::string & host, unsigned short port)
    :m_host(host)
    ,m_port(port)
{

}

int HttpServer::Execute()
{
    event_base_dispatch(m_base);
    return 0;
}

void HttpServer::_Run()
{
    Execute();
}

int HttpServer::Initialize()
{

    struct event_config * ev_config = event_config_new();
    if (!ev_config) 
    {
        perror("Couldn't create config");
        return -1;
    }
    event_config_require_features(ev_config, EV_FEATURE_ET);
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    m_base = event_base_new_with_config(ev_config);
    if (!m_base) 
    {
        perror("Couldn't create event_base");
        return -1;
    }
    free(ev_config);
    m_httpd  = evhttp_new(m_base);
    evhttp_bind_socket(m_httpd, m_host.c_str(), m_port);

    //evhttp_set_timeout(m_httpd, timeout);
    evhttp_set_gencb(m_httpd, httpd_handler, NULL);
    return 0;
}

int HttpServer::Stop()
{
    event_base_loopbreak(m_base);
    return Thread::Stop();
}

int HttpServer::Finalize()
{
    if(m_httpd)
    {
        evhttp_free(m_httpd);
    }

    if(m_base)
    {
        event_base_free(m_base);
    }

    return 0;
}


void HttpServer::httpd_handler(struct evhttp_request *req, void *arg) 
{
    sWebAppliacation->Execute(req);
}



