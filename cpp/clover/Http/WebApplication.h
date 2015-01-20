#pragma once

#include <pcre.h>  

#include "HttpRequest.h"
#include "RequestHandler.h"
#include "Singleton.h"
    

class WebApplication
{
    const char * internal_error = "Internal Server Error";
    const char * not_found = "Not Found";
    const char * not_allowed = " Method Not Allowed";
    
public:

    typedef RequestHandler * (*ReqHandlerCreator)();

    struct URLRoute
    {
        pcre* re;
        //pcre_extra *reExtra;
        std::string name;
        ReqHandlerCreator reqHanCreator;
        int method;

        URLRoute(const std::string & name_, pcre * re_, int method_, ReqHandlerCreator reqHanCreator_)
            :name(name_)
            ,re(re_)
            ,method(method_)
            ,reqHanCreator(reqHanCreator_)
        {

        }

        ~URLRoute()
        {
            if (re)
            {
                pcre_free(re);
            }
        }
    };

    WebApplication() {}
    ~WebApplication();

    
    void Execute(evhttp_request * evReq);

    int Register(const std::string & name, const std::string & urlRegex, int method,  ReqHandlerCreator reqHanCreator);


private:
    
    std::map<std::string, URLRoute*> m_reqHandlers;
};


#define sWebAppliacation Singleton < WebApplication >::Instance()

#define REG_REQHANDLER(name, urlRegex, method, reqHanCreator) \
    sWebAppliacation->Register(name, urlRegex, method, reqHanCreator)

