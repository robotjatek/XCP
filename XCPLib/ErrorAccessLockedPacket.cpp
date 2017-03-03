#include "ErrorAccessLockedPacket.h"



ErrorAccessLockedPacket::ErrorAccessLockedPacket() : ErrorPacket()
{
	m_Data = new uint8_t[1];
	m_Data[BytePositions::ErrorCode] = ErrorCodes::ERR_ACCESS_LOCKED;
	m_DataLength = 1;
	m_PacketSize = 2;
}


ErrorAccessLockedPacket::~ErrorAccessLockedPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void ErrorAccessLockedPacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
