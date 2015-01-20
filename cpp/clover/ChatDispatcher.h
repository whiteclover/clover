#pragma once

#include <map>

#include "NetPacket.h"
#include "Session.h"
#include "Singleton.h"

class ChatDispatcher
{	
public:
	typedef void (ChatSession::*HandleFunc)(NetPacket *); 

	ChatDispatcher() {};
	~ChatDispatcher() {};

	void Execute(Session * session, NetPacket * pack)
	{	
		ChatSession *real = dynamic_cast< ChatSession* >(session);
		uint16 opcode = pack->Opcode();
		auto it = m_funcs.find(opcode);
		if(it != m_funcs.end())
		{
			(real->*it->second)(pack);
		}
		else
		{
			real->Reset();
		}
	}

	void Register(uint16 opcode, HandleFunc func)
	{
		m_funcs[opcode] = func;
	}

private:

	std::map<uint16, HandleFunc> m_funcs;
	
};


#define sChatDispatcher Singleton< ChatDispatcher >::Instance()

#define REG_HANDLER(opcode, func) sChatDispatcher->Register(opcode, func)