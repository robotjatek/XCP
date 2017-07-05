#pragma once
#include "IXCPMessage.h"

class CANMessage final : public IXCPMessage
{
public:
	CANMessage(IXCPPacket* Packet);
	virtual ~CANMessage();
};