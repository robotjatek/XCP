#pragma once
#include "ErrorPacket.h"
class ErrorOutOfRangePacket final :
	public ErrorPacket
{
public:
	ErrorOutOfRangePacket();
	virtual ~ErrorOutOfRangePacket();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

