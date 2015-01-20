#pragma once


#include "Common.h"
#include <event2/event.h>
#include <event2/http.h>

#include "Thread.h"

class HttpServer: public Thread
{
public:
	HttpServer(const std::string & host, unsigned short port);
	~HttpServer() {};

	static void httpd_handler(struct evhttp_request *req, void *arg);

	virtual int Stop();

	virtual int Initialize() override;
	virtual int Execute() override;
	virtual int Finalize();
	virtual void _Run() override;

private:

	struct event_base* 		m_base;
	struct evhttp* 			m_httpd;
	unsigned short          m_port;
	std::string             m_host;
};


