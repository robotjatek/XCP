#include "TCPMessageFactory.h"


TCPMessageFactory::TCPMessageFactory()
{
	m_NumberOfCreatedMessages = 0;
}


TCPMessageFactory::~TCPMessageFactory()
{
}

IXCPMessage * TCPMessageFactory::CreateMessage(IXCPPacket* packet)
{
	return new TCPMessage(packet, m_NumberOfCreatedMessages++);
}

uint8_t TCPMessageFactory::GetHeaderSize()
{
	return 4;
}

uint8_t TCPMessageFactory::GetTailSize()
{
	return 0;
}
