#include "WebApplication.h"

#include <event2/bufferevent.h>

//#include "HttpRequest.h"

WebApplication::~WebApplication()
{
    for (auto it = m_reqHandlers.begin(); it != m_reqHandlers.end(); it++)
    {
        delete it->second;
    }
}

int WebApplication::Register(const std::string & name, const std::string & urlRegex, int method,  ReqHandlerCreator reqHanCreator)
{
    pcre *reCompiled;
    // Optimize the regex
    //pcre_extra *pcreExtra;
    const char *pcreErrorStr;
    int pcreErrorOffset;

    auto it = m_reqHandlers.find(name);
    if (it != m_reqHandlers.end())
    {
        printf("ERROR: You had register the route:'%s', could not register again\n", name.c_str());
        return -1;
    }
    // First, the regex string must be compiled.
    reCompiled = pcre_compile(urlRegex.c_str(), 0, &pcreErrorStr, &pcreErrorOffset, NULL);

    /* OPTIONS (second argument) (||'ed together) can be:
        PCRE_ANCHORED       -- Like adding ^ at start of pattern.
        PCRE_CASELESS       -- Like m//i
        PCRE_DOLLAR_ENDONLY -- Make $ match end of string regardless of \n's
                              No Perl equivalent.
        PCRE_DOTALL         -- Makes . match newlins too.  Like m//s
        PCRE_EXTENDED       -- Like m//x
        PCRE_EXTRA          -- 
        PCRE_MULTILINE      -- Like m//m
        PCRE_UNGREEDY       -- Set quantifiers to be ungreedy.  Individual quantifiers
                              may be set to be greedy if they are followed by "?".
        PCRE_UTF8           -- Work with UTF8 strings.
    */

    // pcre_compile returns NULL on error, and sets pcreErrorOffset & pcreErrorStr
    if(reCompiled == NULL)
    {
        printf("ERROR: Could not compile '%s': %s\n", urlRegex.c_str(), pcreErrorStr);
        return -1;
    }

    m_reqHandlers[name] = new URLRoute(name, reCompiled, method, reqHanCreator);
    return 0;
}

void  WebApplication::Execute(evhttp_request * evReq)
{
    int pcreExecRet;
    int subStrVec[30];
    //const char *pcreErrorStr;
    bool matched = false;
    struct evkeyvalq *outHeaders;

    const char *uri = evhttp_request_get_uri (evReq);
    int method =  evhttp_request_get_command(evReq);

    outHeaders = evhttp_request_get_output_headers(evReq);
    evhttp_add_header(outHeaders, "Server" , "Clover/0.1");
    
    printf("Web:Request : %s, method:%d\n", uri, method);

    for (auto it = m_reqHandlers.begin(); it != m_reqHandlers.end(); it++)
    {
        /* Try to find the regex in current path_info, and report results. */
        pcreExecRet = pcre_exec(it->second->re,
                                NULL,
                                uri, 
                                strlen(uri),            // length of string
                                0,                      // Start looking at this point
                                0,                      // OPTIONS
                                subStrVec,
                                30);                    // Length of subStrVec
        /* If match */
        if(pcreExecRet >= 0)
        {
            if(it->second->method == method)
            {
                HttpRequest * httpReq = new HttpRequest(evReq);
                RequestHandler * reqHandler = it->second->reqHanCreator();
                reqHandler->InitHttpReq(httpReq);
                reqHandler->_Execute();
                delete reqHandler;
                delete httpReq;
                return;
            }
            matched = true;
        }
    }

    struct evbuffer *evb = NULL;
    evb = evbuffer_new ();
    /* Method not allowed */
    if (matched)
    {
        evbuffer_add (evb, not_allowed, strlen(not_allowed));
        evhttp_send_reply (evReq, 405, not_allowed, evb);
    }
    else
    {    
        evbuffer_add (evb, not_found, strlen(not_found));
        evhttp_send_reply (evReq, 403, not_found, evb);
    }
    evbuffer_free(evb);
}