#include "Room.h"


int Room::AddPeer(Peer * peer)
{
    ScopeMutex lock(m_lock);
    if (Size() < m_max_members)
    {
        auto it = m_members.find(peer->SessionId());
        if (it != m_members.end())
        {
            return 0;
        }
        m_members[peer->SessionId()] = peer;
        return 0;
    }
    return -1;
}

int Room::RemovePeer(int seed_id)
{
    ScopeMutex lock(m_lock);
    auto it = m_members.find(seed_id);
    if (it != m_members.end())
    {
        Peer * p = it->second;
        m_members.erase(it);
        delete p;
        return 0;
    }
    return -1;
}

Peer * Room::Find(int seed_id)
{
    auto it = m_members.find(seed_id);
    if (it != m_members.end())
    {
       return it->second;
    }
    return NULL;
}

unsigned int Room::RoomId() const { return m_roomId; }

size_t Room::Size() const { return m_members.size(); }

void Room::Broadcast(const NetPacket & packet)
{
    auto it = m_members.begin();
    for ( ; it != m_members.end(); it++)
    {
        it->second->Send(packet);
    }
}


void Room::Broadcast(const NetPacket & packet, int session_id)
{
    auto it = m_members.begin();
    for ( ; it != m_members.end(); it++)
    {
        if (it->second->SessionId() != session_id)
        {
            it->second->Send(packet);
        }    
    }
}


int RoomMananger::AddRoom(Room * room)
{
    ScopeMutex lock(m_lock);
    if (Size() < m_max_room)
    {
        auto it = m_rooms.find(room->RoomId());
        if (it != m_rooms.end())
        {
            return 0;
        }
        m_rooms[room->RoomId()] = room;
        return 0;
    }
    return -1;
}


int RoomMananger::RemoveRoom(unsigned int room_id)
{
    ScopeMutex lock(m_lock);
    auto it = m_rooms.find(room_id);
    if (it != m_rooms.end())
    {

        m_rooms.erase(it);
        return 0;
    }
    return -1;
}

size_t RoomMananger::Size() const { return m_rooms.size(); }

Room * RoomMananger::Find(unsigned int room_id)
{
    auto it = m_rooms.find(room_id);
    if (it != m_rooms.end())
    {
       return it->second;
    }
    return NULL;
}