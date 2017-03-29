#pragma once
#include "ErrorPacket.h"
class ErrorMemoryOverflowPacket final :
	public ErrorPacket
{
public:
	ErrorMemoryOverflowPacket();
	virtual ~ErrorMemoryOverflowPacket();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

