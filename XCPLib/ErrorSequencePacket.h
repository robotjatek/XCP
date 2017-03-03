#pragma once
#include "ErrorPacket.h"
class ErrorSequencePacket :
	public ErrorPacket
{
public:
	ErrorSequencePacket();
	virtual ~ErrorSequencePacket();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

