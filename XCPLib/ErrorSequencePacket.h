#pragma once
#include "ErrorPacket.h"
class ErrorSequencePacket final :
	public ErrorPacket
{
public:
	ErrorSequencePacket();
	virtual ~ErrorSequencePacket();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

