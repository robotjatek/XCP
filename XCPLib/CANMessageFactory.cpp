#include "CANMessageFactory.h"
#include "CANMessage.h"


CANMessageFactory::CANMessageFactory()
{
}


CANMessageFactory::~CANMessageFactory()
{
}

uint8_t CANMessageFactory::GetHeaderSize()
{
	return 0;
}

uint8_t CANMessageFactory::GetTailSize()
{
	return 0;
}

IXCPMessage * CANMessageFactory::CreateMessage(IXCPPacket * packet)
{
	return new CANMessage(packet);
}
