#include "ErrorSequencePacket.h"



ErrorSequencePacket::ErrorSequencePacket() : ErrorPacket()
{
	m_Data = new uint8_t[1];
	m_Data[0] = ErrorCodes::ERR_SEQUENCE;
	m_DataLength = 1;
	m_PacketSize = 2;
}


ErrorSequencePacket::~ErrorSequencePacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void ErrorSequencePacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
