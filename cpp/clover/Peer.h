#pragma once

#include "Session.h"
#include "NetPacket.h"

class Peer
{
public:
	Peer(unsigned int uid, Session * session)
		:m_uid(uid)
		,m_session(session)
	{

	}

	~Peer()
	{
		printf("Peer : %d delete\n", m_uid );
		m_session = NULL;
	}


	int SessionId() const { return m_session->SessionId(); }

	void Send(const NetPacket & pack)
	{
		m_session->Send(pack);
	}

private:

	unsigned int m_uid;
	Session * m_session;

};