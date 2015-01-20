#pragma once

class Session;

class INetHandler
{
public:
    INetHandler() {};
    ~INetHandler() {};

    virtual void OnConnect(Session * session) = 0;
    virtual void OnRead(Session *session, NetPacket* pNetPack) = 0;
    virtual void OnDisconnect(Session * session) = 0;

    virtual Session * CreateNewSession() = 0;
};
