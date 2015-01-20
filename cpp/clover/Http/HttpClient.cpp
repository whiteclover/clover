#include "HttpClient.h"


HttpClient::HttpClient(struct event_base * base)
    :m_base(base)
    ,m_evConn(NULL)
    ,m_evReq(NULL)
    ,m_resCb(NULL)
{

}

HttpClient::~HttpClient()
{
    Clear();
}

void HttpClient::Clear()
{
    if(m_evConn)
    {
        evhttp_connection_free(m_evConn);
        m_evConn = NULL;
    }   
    if(m_reqInfo)
    {
        delete m_reqInfo;
        m_reqInfo = NULL;
    }
}

void  HttpClient::request_callback(struct evhttp_request *evRes, void *data)
{
    HttpClient * client = static_cast<HttpClient *> (data);
    ResponseInfo *resInfo = NULL;
    
    if(evRes)
    {
        resInfo = new ResponseInfo(evRes);
    }

    if (client->m_resCb)
    {
        client->m_resCb(resInfo, client);
    }
    else
    {
        switch(evRes->response_code)
        {
        case HTTP_OK:
        {
            // To do something
            client->Clear();
            break;
        }
        case HTTP_MOVEPERM:
        case HTTP_MOVETEMP:
        {
            const char * new_location = evhttp_find_header(evRes->input_headers, "Location");
            if(!new_location) 
            {
                return;
            }
            evhttp_uri* new_uri = evhttp_uri_parse(new_location);
            if(!new_uri)
            {
                return;
            }
            client->m_reqInfo->Uri(new_uri);
            //delete resInfo;
            client->Send(client->m_reqInfo, client->m_resCb);
            return;
        }

        default: { break ;}

        }
    }
    //delete resInfo;
}

void HttpClient::Send(RequestInfo *reqInfo, ResponseCallback resCallback)
{

    m_reqInfo = reqInfo;

    m_resCb = resCallback;
    std::string uri; // char uri[256];
    int method, port;

    evhttp_uri * evUri = m_reqInfo->Uri();
    port = evhttp_uri_get_port(evUri);
    if(port == -1)
    {
        port = 80;
    }

    method = m_reqInfo->Method();

    const char * host = evhttp_uri_get_host(evUri);
    const char * path =  evhttp_uri_get_path(evUri);
    if (path == NULL) 
    {
        path = "/";
    }

    const char * query = evhttp_uri_get_query(evUri);
    if (query == NULL) 
    {
        uri.append(path);
        //snprintf(uri, sizeof(uri) - 1, "%s", path);
    } 
    else 
    {
        uri.append(path);
        uri.append("?");
        uri.append(query);
        //snprintf(uri, sizeof(uri) - 1, "%s?%s", path, query);
    }

    printf("Host:%s, path: %s\n", host, path );

    if(m_evConn)
    {
        evhttp_connection_free(m_evConn);
        m_evConn = NULL;
    }

    m_evConn = evhttp_connection_base_new(m_base, NULL, host, port);
    m_evReq = evhttp_request_new(request_callback, this);

    // Try 
    if(m_reqInfo->BodySize() && method == EVHTTP_REQ_GET)
    {
        method = EVHTTP_REQ_POST;
    }

    switch(method)
    {
    case EVHTTP_REQ_POST:
    case EVHTTP_REQ_PUT:
    case EVHTTP_REQ_DELETE:
    {
        evbuffer_add(m_evReq->output_buffer, m_reqInfo->m_body.c_str(), m_reqInfo->BodySize());
        break;
    }
    default: { break; }

    }

    struct evkeyvalq *output_headers = evhttp_request_get_output_headers(m_evReq);
    std::map<std::string, std::string> headers;
    GenDefaultHeaders(headers);

    headers.insert(m_reqInfo->m_headers.begin(), m_reqInfo->m_headers.end());
    for (auto it = headers.begin(); it != headers.end(); it++)
    {
        evhttp_add_header(output_headers, it->first.c_str(), it->second.c_str());
    }
    evhttp_add_header(output_headers, "Host", host);
    evhttp_add_header(output_headers, "Connection", "close");
    printf("evhttp_make_request\n");

    int rc;
    rc = evhttp_make_request(m_evConn, m_evReq, method, uri.c_str());
    if (rc != 0) {
        fprintf(stderr, "evhttp_make_request() failed\n");
        return ;
    }
}


/************************************
*
*   RequestInfo
*
*************************************/

RequestInfo::RequestInfo(const std::string & url)
        :m_method(EVHTTP_REQ_GET)
{
    m_evUri = evhttp_uri_parse(url.c_str());  
}

RequestInfo::RequestInfo(const char * url)
        :m_method(EVHTTP_REQ_GET)
{
    m_evUri = evhttp_uri_parse(url);  
}


RequestInfo::RequestInfo(const std::string & url, const std::string & body)
        :m_method(EVHTTP_REQ_POST)
        ,m_body(body)
{
        m_evUri = evhttp_uri_parse(url.c_str());  
}


RequestInfo::RequestInfo(const char * url, const std::string & body)
    :m_method(EVHTTP_REQ_POST)
    ,m_body(body)
{
    m_evUri = evhttp_uri_parse(url);  
}


RequestInfo::~RequestInfo()
{
    if(m_evUri)
    {
        evhttp_uri_free(m_evUri);  
        m_evUri = NULL; 
    }
}


void RequestInfo::Uri(const std::string & url) 
{
    if(m_evUri)
    {
        evhttp_uri_free(m_evUri);  
        m_evUri = evhttp_uri_parse(url.c_str());  
    }
}

void RequestInfo::Uri(struct evhttp_uri * evUri) 
{
    if(m_evUri)
    {
        evhttp_uri_free(m_evUri);  
        m_evUri = evUri;
    }
}

void RequestInfo::Uri(const char * url) 
{
    if(m_evUri)
    {
        evhttp_uri_free(m_evUri);  
        m_evUri = evhttp_uri_parse(url);  
    }
}


bool RequestInfo::AddHeader(const std::string & key, const std::string & value)
{
    auto it = m_headers.find(key);
    if(it == m_headers.end())
    {
        m_headers.insert(std::pair<std::string, std::string>(key, value));
        return true;
    }

    return false;
}


bool RequestInfo::SetHeader(const std::string & key, const std::string & value)
{
    m_headers[key]  = value;
    return true;
}


/************************************
*
*   ResponseInfo
*
*************************************/

ResponseInfo::ResponseInfo(struct evhttp_request * evResInfo)
    : m_evRes(evResInfo)
{
    struct evbuffer* buf;
    buf = evhttp_request_get_input_buffer(m_evRes);  
    if (buf)
    {
        size_t len = evbuffer_get_length(buf);    
        if (len > 0 )
        {   
            char *body = (char*)malloc(len+1);  
            memcpy(body, evbuffer_pullup(buf, -1), len);  
            body[len] = '\0';  
            m_body.assign(body);
            free(body);  
        }
    }
}


bool ResponseInfo::Header(const std::string &key, std::string &value)
{
    // Lazy method
    _GenHeaders();
    auto it = m_headers.find(key);
    if (it == m_headers.end())
    {
        value.assign(it->second);
        return true;
    }
    return false;
}

void ResponseInfo::_GenHeaders()
{
    struct evkeyvalq *headers;
    struct evkeyval *header;

    if (m_genHead)
    {
        return ;
    }

    headers = evhttp_request_get_input_headers(m_evRes);
    for (header = headers->tqh_first; header; header = header->next.tqe_next) 
    {
        std::string key(header->key);
        std::string value(header->value);
        m_headers[key] = value;
    }

    m_genHead = true;
}