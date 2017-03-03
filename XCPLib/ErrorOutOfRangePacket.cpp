#include "ErrorOutOfRangePacket.h"



ErrorOutOfRangePacket::ErrorOutOfRangePacket() : ErrorPacket()
{
	m_Data = new uint8_t[1];
	m_Data[0] = ErrorCodes::ERR_OUT_OF_RANGE;
	m_DataLength = 1;
	m_PacketSize = 2;
}


ErrorOutOfRangePacket::~ErrorOutOfRangePacket()
{
	delete[] m_Data;
}

void ErrorOutOfRangePacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
