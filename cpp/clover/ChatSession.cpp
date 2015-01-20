
#include "Common.h"
#include "ChatSession.h"

#include "Room.h"

void ChatSession::HandlePing(NetPacket * recvPack)
{
	NetPacket pack((uint16)(recvPack->Opcode() +1));
    
    std::string s = "Pong";
    *recvPack >> s;
    pack << s;
    Send(pack);
}

void ChatSession::HandleEnterRoom(NetPacket * recvPack)
{
	unsigned int uid, room_id;
	*recvPack >> uid;
	*recvPack >> room_id;

	Room * room = sRoomMgr->Find(room_id);
	if (room == NULL)
	{
		room = new Room("test", room_id, 50);
		sRoomMgr->AddRoom(room);
	}
	Peer * peer = new Peer(uid, this);
	room->AddPeer(peer);
	NetPacket pack((uint16)(recvPack->Opcode() +1));
	std::string msg = "User: " + std::to_string (uid) + "  Enter Room: " + std::to_string(room_id);
	pack << msg;
	m_roomId = room_id;
	room->Broadcast(pack, SessionId());
	//peer->Send(room->History());
}

void ChatSession::HandleLevaeRoom(NetPacket * recvPack)
{
	unsigned int uid, room_id;

	*recvPack >> uid;
	*recvPack >> room_id;

	Room * room = sRoomMgr->Find(room_id);
	if (room != NULL)
	{
		room->RemovePeer(SessionId());
		NetPacket pack((uint16)(recvPack->Opcode() +1));
		std::string msg = "User: " + std::to_string (uid) + "  Leave Room: " + std::to_string(room_id);
		pack << msg;
		room->Broadcast(pack);
	}
}

void ChatSession::HandlePlayerMessage(NetPacket * recvPack)
{
	unsigned int uid, room_id;
	std::string messsage;

	*recvPack >> uid;
	*recvPack >> room_id;
	*recvPack >> messsage;

	Room * room = sRoomMgr->Find(room_id);
	if (room != NULL)
	{
		NetPacket pack((uint16)(recvPack->Opcode() +1));
		pack << messsage;
		room->Broadcast(pack, SessionId());
	}
}
