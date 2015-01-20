#include "RequestHandler.h"

void RequestHandler::_Execute()
{
	Initialize();
	Execute();
	m_httpReq->Send(m_status_code, m_reason, m_sendHeaders, m_body);
	
	Finalize();
}


void RequestHandler::Write(const std::string & chunk)
{
	m_body.append(chunk);
}

int RequestHandler::Initialize()
{
	//printf("RequestHandler::Initialize\n");
	return 0;
}

int RequestHandler::Finalize()
{
	return 0;
}
