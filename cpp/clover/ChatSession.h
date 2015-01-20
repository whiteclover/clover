#pragma once

#include "Session.h"
#pragma once

#include "NetPacket.h"

class ChatSession: public Session
{
public:


    void HandlePing(NetPacket * NetPacket);
    void HandleEnterRoom(NetPacket * recvPack);
    void HandleLevaeRoom(NetPacket * recvPack);
    void HandlePlayerMessage(NetPacket * recvPack);


    unsigned int RoomId() const { return m_roomId; }


private:
    bool authenticated;

    unsigned int  m_roomId;

};