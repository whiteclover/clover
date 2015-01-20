#pragma once

#include "Common.h"
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>


typedef std::map<std::string, std::string> HttpHeaders;

class HttpRequest
{

    const char * internal_error = "Internal Server Error";
    const char * not_found = "Not Found";
    const char * not_allowed = " Method Not Allowed";

public:


    HttpRequest(evhttp_request * evReq)
        :m_evReq(evReq)
    {
        m_genHead = false;
    }

    ~HttpRequest(){}
    
    int  Method();

    // std::string PathInfo()
    // {
    //  struct evbuffer *evb = NULL;
    // }
    //void GetParam(std::string & key, std::string & value);
    //std::string * GetParams(std::string & key);

    bool Header(std::string &key, std::string &value)
    {
        // Lazy method
        _GenHeaders();
        auto it = m_headers.find(key);
        if (it == m_headers.end())
        {
            value += it->second;
            return true;
        }
        return false;
    }

    int Send(unsigned short status_code, std::string & status_msg, HttpHeaders & headers, std::string & body);
    //int Send(unsigned short status_code, std::string & status_msg, HttpHeaders & headers, std::sting & body, std::string & contentType);
    //int Fulsh();

private:

    void _GenHeaders();

private:

    evhttp_request *        m_evReq;
    HttpHeaders             m_headers;
    bool                    m_genHead;

    // For stats
    // unsigned long           m_startTime;
    // unsigned long           m_finshTime;
};


