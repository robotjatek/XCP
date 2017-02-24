#pragma once
#include "IXCPMessage.h"
#include "IXCPPacket.h"

class IMessageFactory
{
public:
	IMessageFactory() {}
	virtual ~IMessageFactory() {}
	virtual IXCPMessage* CreateMessage(IXCPPacket* packet) = 0;
	virtual uint8_t GetHeaderSize() = 0;
	virtual uint8_t GetTailSize() = 0;
};

