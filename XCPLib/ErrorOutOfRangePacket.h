#pragma once
#include "ErrorPacket.h"
class ErrorOutOfRangePacket :
	public ErrorPacket
{
public:
	ErrorOutOfRangePacket();
	virtual ~ErrorOutOfRangePacket();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

