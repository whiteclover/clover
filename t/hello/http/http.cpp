#include "HttpServer.h"
#include "HttpClient.h"
#include "RequestHandler.h"
#include "WebApplication.h"


class Hello : public RequestHandler
{

public:

    Hello()
    {
        printf("RequestHandler::RequestHandler\n");
    }
    static RequestHandler* Instance() { return new  Hello(); } ;

    virtual void Execute() override;

};

void Hello::Execute()
{
    std::string text("Hello");
    Write(text);
}


class Index : public RequestHandler
{

public:

    // Index()
    // {
    // }
    static RequestHandler* Instance() { return new  Index(); } ;

    virtual void Execute() override;

};


void Index::Execute()
{
    std::string text("Index");
    Write(text);
}


void callback(ResponseInfo* info, void* userdata)
{
    HttpClient * client = static_cast<HttpClient *> (userdata);

    printf("Body:%s\n", info->Body().c_str());
    client->Clear();
    client->Stop();
    delete client;
}

int test_client(const char * uri)
{
    struct event_base* base = event_base_new();
    HttpClient *client= new HttpClient(base);
    RequestInfo * req = new RequestInfo(uri);
    req->AddHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    req->AddHeader("Connection", "keep-alive");

    client->Send(req, callback);
    event_base_dispatch(base);  
    return 0;
}

int main(int argc, char **args)
{

    REG_REQHANDLER("hello", "^/hello$", EVHTTP_REQ_GET, &Hello::Instance);
    REG_REQHANDLER("index", "^/$", EVHTTP_REQ_GET, &Index::Instance);
    REG_REQHANDLER("index2", "^/index$", EVHTTP_REQ_GET, &Index::Instance);
    
    std::string host = "127.0.0.1";
    unsigned short port = 8080;
    HttpServer httpd = HttpServer(host, port);

    httpd.Run();


    if (args >= 2)
    {
        test_client(args[1]);
    }
    else
    {
        perror("Must have a arugement");
    }
    
    return 0;

}