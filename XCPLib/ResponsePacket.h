#pragma once
#include "ICTO.h"

class ResponsePacket : public ICTO
{
public:
	ResponsePacket() : ICTO()
	{
		m_PID = CTOSlaveToMasterPacketTypes::RES;
	}

	virtual ~ResponsePacket() {}
	virtual void Dispatch(IIncomingMessageHandler& Handler)
	{
		Handler.Handle(*this);
	}
};