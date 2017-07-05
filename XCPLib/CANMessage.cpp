#include "CANMessage.h"

CANMessage::CANMessage(IXCPPacket * Packet) : IXCPMessage()
{
	m_Packet = Packet;
	m_Header = nullptr;
	m_Tail = nullptr;
}

CANMessage::~CANMessage()
{
	//deletion is handled in the parent class
}