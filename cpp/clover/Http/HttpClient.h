#pragma once

#include "Common.h"

#include <event2/event.h>  
#include <event2/buffer.h>  
#include <event2/http.h>  
#include <event2/http_struct.h>  
#include <event2/keyvalq_struct.h>  

typedef std::map<std::string, std::string> Headers;

class RequestInfo
{
	friend class HttpClient;

public:

	RequestInfo(const std::string & url);
	RequestInfo(const char * url);
	RequestInfo(const std::string & url, const std::string & body);
	RequestInfo(const char * url, const std::string & body);
	~RequestInfo();

	void Uri(const std::string & url);
	void Uri(struct evhttp_uri * evUri);
	void Uri(const char * url);
	evhttp_uri * Uri() { return m_evUri; }

	inline int Method() const  { return m_method; }
	void Method(int method) { m_method = method; }


	inline void Body(const std::string & body) { m_body.assign(body); }
	inline int BodySize() const { return m_body.length(); }

	bool AddHeader(const std::string & key, const std:: string & value);
	bool SetHeader(const std::string & key, const std::string & value);

private:

	Headers 				m_headers;
	struct evhttp_uri * 	m_evUri;  
	int 					m_method;
	std::string 			m_body;

};


class ResponseInfo
{

public:
	friend class HttpClient;

	ResponseInfo(struct evhttp_request * evResInfo);
	~ResponseInfo() {}

	inline int StatusCode() const { return m_evRes->response_code;}

	inline const std::string & Body() const { return m_body;}

	bool Header(const std::string &key, std::string &value);

private:

	void _GenHeaders();

	struct evhttp_request *     m_evRes;
	Headers 					m_headers;
	std::string 				m_body;
	bool 						m_genHead;
};



class HttpClient
{

public:

	typedef void (*ResponseCallback)(ResponseInfo *, void * data);

	HttpClient(struct event_base * base);
	~HttpClient();

	inline void Stop() {  event_base_loopbreak(m_base); }

	RequestInfo * GetRequestInfo() { return m_reqInfo; }

	void Clear();
	void Send(RequestInfo * reqInfo, ResponseCallback resCallback);

protected:

	static void request_callback(struct evhttp_request *evRes, void *data);

	void GenDefaultHeaders( Headers& headers)
	{
		headers.insert(std::pair<std::string, std::string>("User-Agent", "Clover/0.1"));
	}

	struct event_base * 		m_base;
	struct evhttp_connection * 	m_evConn;
	struct evhttp_request *		m_evReq;
	RequestInfo * 				m_reqInfo;
	ResponseCallback 			m_resCb;
};