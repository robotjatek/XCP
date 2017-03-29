#pragma once
#include "IMessageFactory.h"
#include "TCPMessage.h"

class TCPMessageFactory final : public IMessageFactory
{
private:
	uint16_t m_NumberOfCreatedMessages;
public:
	TCPMessageFactory();
	virtual ~TCPMessageFactory();
	virtual IXCPMessage* CreateMessage(IXCPPacket* packet);
	virtual uint8_t GetHeaderSize();
	virtual uint8_t GetTailSize();
};

