#pragma once

#include "Common.h"
#include "NetPacket.h"
#include "Mutex.h"
#include "Peer.h"
#include "Singleton.h"

class Room
{
    friend class RoomMananger;

public:
    Room(const std::string& name,  unsigned int room_id, unsigned short max_members)
        :m_name(name)
        ,m_roomId(room_id)
        ,m_max_members(max_members) 
    { 
    }

    ~Room() {}

    int AddPeer(Peer * peer);
    int RemovePeer(int seed_id);
    unsigned int RoomId() const;
    Peer * Find(int seed_id);
    size_t Size() const;
    
    void Broadcast(const NetPacket & packet);
    void Broadcast(const NetPacket & packet, int session_id);


private:
    std::vector<NetPacket>      history;
    Mutex                       m_lock;
    unsigned short              m_max_members;
    std::string                 m_name;
    unsigned int                m_roomId;
    std::map<int, Peer*>        m_members;
};


class RoomMananger
{
public:
    RoomMananger(unsigned int max_room=1000) :m_max_room(max_room) {}
    ~RoomMananger() {}

    int AddRoom(Room * room);
    int RemoveRoom(unsigned int room_id);
    size_t Size() const;
    Room * Find(unsigned int room_id);

private:
    unsigned int            m_max_room;
    Mutex                   m_lock;
    std::map <int, Room*>    m_rooms;
};


#define sRoomMgr  Singleton< RoomMananger >::Instance()