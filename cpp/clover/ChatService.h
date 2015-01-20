#pragma once

#include "Listener.h"

class ChatService
{
public:
    ChatService();
    ~ChatService();

    void Run();
    void Stop();

private:

    int Bootstrap();
    void BootstrapDispatcher();

private:
    Listener m_listener;
};