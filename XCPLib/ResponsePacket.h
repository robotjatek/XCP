#pragma once
#include "ICTO.h"

class ResponsePacket : public ICTO
{
public:
	ResponsePacket() : ICTO()
	{
		m_PID = CTOSlaveToMasterPacketTypes::RES;
		m_PacketSize = 1;
		m_DataLength = 0;
		m_Data = nullptr;
	}

	virtual ~ResponsePacket() {}
	virtual void Dispatch(IIncomingMessageHandler& Handler)
	{
		Handler.Handle(*this);
	}
};