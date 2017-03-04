#pragma once
#include "ErrorPacket.h"
class ErrorMemoryOverflowPacket :
	public ErrorPacket
{
public:
	ErrorMemoryOverflowPacket();
	virtual ~ErrorMemoryOverflowPacket();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

