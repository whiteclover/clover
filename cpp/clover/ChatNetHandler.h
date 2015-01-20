#pragma once

#include "INetHandler.h"
#include "ChatSession.h"
#include "Room.h"

class ChatNetHandler: public INetHandler
{
public:
	ChatNetHandler(){}
	~ChatNetHandler(){};
	
   	virtual void OnConnect(Session * session) override;
	virtual void OnRead(Session *conn, NetPacket* pNetPack) override;
	virtual void OnDisconnect(Session * session) override;	

	virtual Session * CreateNewSession() override;
};


void ChatNetHandler::OnRead(Session *session, NetPacket * pNetPack)
{
	sChatDispatcher->Execute(session, pNetPack);
	delete pNetPack;
}

Session * ChatNetHandler::CreateNewSession()
{
	return new ChatSession;
}

	
void ChatNetHandler::OnConnect(Session * session)
{
	return ;
}

void ChatNetHandler::OnDisconnect(Session * session)
{
	ChatSession * real = dynamic_cast< ChatSession * > (session);
	Room * room = sRoomMgr->Find(real->RoomId());
	if (room != NULL)
	{
		printf("User Leave:%d\n", real->SessionId() );
		room->RemovePeer(real->SessionId());
	} 
}