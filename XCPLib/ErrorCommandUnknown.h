#pragma once
#include "ErrorPacket.h"
class ErrorCommandUnknown final :
	public ErrorPacket
{
public:
	ErrorCommandUnknown();
	virtual ~ErrorCommandUnknown();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

