#pragma once
#include "ErrorPacket.h"
class ErrorAccessLockedPacket :
	public ErrorPacket
{
private:
	enum BytePositions
	{
		ErrorCode = 0x0,
	};
public:
	ErrorAccessLockedPacket();
	virtual ~ErrorAccessLockedPacket();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

