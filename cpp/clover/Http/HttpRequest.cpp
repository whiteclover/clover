#include "HttpRequest.h"



int HttpRequest::Method()
{
	return evhttp_request_get_command(m_evReq);
}


int HttpRequest::Send(unsigned short status_code, std::string & status_msg, HttpHeaders & headers, std::string & body)
{
	struct evkeyvalq *outHeaders;
	outHeaders = evhttp_request_get_output_headers(m_evReq);
	for (auto head  = headers.begin(); head != headers.end(); head++)
	{
		
		evhttp_add_header(outHeaders, head->first.c_str(), head->second.c_str());
	}
	
	struct evbuffer *evb = NULL;
	evb = evbuffer_new ();
	if (evb)
	{
		evbuffer_add (evb,  body.c_str(), body.length());
		evhttp_send_reply (m_evReq, status_code, status_msg.c_str(), evb);
		evbuffer_free (evb);
		return 0;
  	}
  	else
  	{
  		evhttp_send_reply (m_evReq, 500, internal_error, NULL);
  	}
  	return -1;
}


void HttpRequest::_GenHeaders()
{
	struct evkeyvalq *headers;
	struct evkeyval *header;

	if (m_genHead)
	{
		return ;
	}

	headers = evhttp_request_get_input_headers(m_evReq);
	for (header = headers->tqh_first; header; header = header->next.tqe_next) 
	{
		std::string key(header->key);
		std::string value(header->value);
		m_headers[key] = value;
	}

	m_genHead = true;
}


