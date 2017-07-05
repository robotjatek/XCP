#pragma once
#include "IMessageFactory.h"
class CANMessageFactory final :	public IMessageFactory
{
public:
	CANMessageFactory();
	virtual ~CANMessageFactory();
	virtual uint8_t GetHeaderSize();
	virtual uint8_t GetTailSize();
	virtual IXCPMessage* CreateMessage(IXCPPacket* packet);
};

