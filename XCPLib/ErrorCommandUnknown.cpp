#include "ErrorCommandUnknown.h"



ErrorCommandUnknown::ErrorCommandUnknown()
{
	m_DataLength = 1;
	m_PacketSize = 2;
	m_Data = new uint8_t[m_DataLength];
	m_Data[0] = ErrorCodes::ERR_CMD_UNKNOWN;
}


ErrorCommandUnknown::~ErrorCommandUnknown()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void ErrorCommandUnknown::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
