#include "ErrorMemoryOverflowPacket.h"



ErrorMemoryOverflowPacket::ErrorMemoryOverflowPacket() : ErrorPacket()
{
	m_Data = new uint8_t[1];
	m_Data[0] = ErrorCodes::ERR_MEMORY_OVERFLOW;
	m_DataLength = 1;
	m_PacketSize = 2;
}


ErrorMemoryOverflowPacket::~ErrorMemoryOverflowPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void ErrorMemoryOverflowPacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
