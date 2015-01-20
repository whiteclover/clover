#include "ChatService.h"
#include "ChatSession.h"
#include "ChatDispatcher.h"
#include "ChatNetHandler.h"
#include "ConnectionPool.h"
#include "ChatNetHandler.h"

#include <event2/thread.h>

ChatService::ChatService()
{

}

ChatService::~ChatService()
{

}

void ChatService::Run()
{
    Bootstrap();

    m_listener.Initialize("127.0.0.1", 9999);
    m_listener.Start();
}


int ChatService::Bootstrap()
{
    //init event thread
    if(evthread_use_pthreads() != 0)
        return -1;

    ChatNetHandler * handler = new ChatNetHandler;

    sConnectionPool->Initialize(handler, 2048*8);

    BootstrapDispatcher();
    return 0;
}


void ChatService::BootstrapDispatcher()
{
    REG_HANDLER(1, &ChatSession::HandlePing);
    REG_HANDLER(3, &ChatSession::HandleEnterRoom);
    REG_HANDLER(5, &ChatSession::HandleLevaeRoom);
    REG_HANDLER(7, &ChatSession::HandlePlayerMessage);
}