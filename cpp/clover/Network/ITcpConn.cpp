#include "ITcpConn.h"


ITcpConn::ITcpConn()
    :m_bev(NULL)
    ,m_evbase(NULL)
    ,m_sockfd(-1)
    ,m_addrlen(0)
    ,m_index(-1)
{

}

ITcpConn::~ITcpConn()
{
    
}

void ITcpConn::Start(struct event_base * base, int  fd, struct sockaddr *address, int addrlen)
{
    m_sockfd = fd;
    m_sockaddr  = *(sockaddr_in*)address;
    m_addrlen = addrlen;
    m_evbase = base;
    evutil_make_socket_nonblocking(m_sockfd);
    struct bufferevent *bev =  bufferevent_socket_new(m_evbase, fd,
                         BEV_OPT_CLOSE_ON_FREE);
    if (bev)
    {
        //printf("Fine\n");
    }
    bufferevent_setcb(bev, ITcpConn::on_read_cb, ITcpConn::on_write_cb, ITcpConn::on_event_cb, this);
    m_bev = bev;
    printf("A client :%d, %p\n", fd, this);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    Prepare();
}

void ITcpConn::EnableEvent(int event)
{
    bufferevent_enable(m_bev, event);
}
void ITcpConn::DisableEvent(int event)
{
    bufferevent_disable(m_bev, event);
}

void ITcpConn::Send(const uint8* data, int size)
{
    bufferevent_write(m_bev, data, size);
    EnableEvent(EV_WRITE);
}


void ITcpConn::on_read_cb(struct bufferevent* bev, void* data)
{
    ITcpConn * con = static_cast<ITcpConn*>(data);
    if(con)
    {   
        con->OnRead(bev);
    }  
}

void ITcpConn::on_write_cb(struct bufferevent* bev, void* data)
{
    ITcpConn * con = static_cast<ITcpConn*>(data);
    if(con)
    {   
        con->OnWrite(bev); 
    }  
}

void ITcpConn::on_event_cb(struct bufferevent * bev, short event, void *data)
{
    ITcpConn * con = static_cast<ITcpConn*>(data);
    if(con)
    {
        con->OnEvent(bev ,event);
    }
    else
    {   if (event & BEV_EVENT_EOF)  
        {  
            /* ITcpConn has been closed, do any clean up here */  
            printf("ITcpConn closed\n");  
        }  
        else if (event & BEV_EVENT_ERROR)  
        {  
            /* check errno to see what error occurred */  
            printf("some other error\n");  
        }  
        else if (event & BEV_EVENT_TIMEOUT)  
        {  
            /* must be a timeout event handle, handle it */  
            printf("Timed out\n");  
        }   
    }
}

