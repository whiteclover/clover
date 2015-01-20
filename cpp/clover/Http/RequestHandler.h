#pragma once

#include "HttpRequest.h"

class WebApplication;


class RequestHandler
{
	
public:

	friend class WebApplication;

	RequestHandler()
		:m_httpReq(NULL)
		,m_status_code(200)
		,m_reason("OK")

	{
		//Reset();
		// m_sendHeaders["Content-Type"] = "text/html; charset=utf-8";
		// m_sendHeaders["Connection"] = "keep-alive";

	}

	HttpRequest * Request() const { return m_httpReq; }

	void InitHttpReq(HttpRequest * httpReq) { m_httpReq = httpReq; }
	virtual int Initialize();
	virtual int Finalize();

	int AddHeader(const std::string &key, const std::string & vaule) {return 0; }
	int DelHeader(const std::string &key){return 0; }
	int SetHeader(const std::string &key, const std::string &value) {return 0; }
	void Write(const std::string & chunk);

	virtual void Execute() = 0;

private:

	void _Execute();


private:
	HttpRequest * 			m_httpReq;
    unsigned short 			m_status_code;
    std::string 			m_reason;
	HttpHeaders 			m_sendHeaders;
	std::string 			m_body;
};