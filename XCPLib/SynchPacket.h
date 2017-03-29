#pragma once
#include "CommandPacket.h"
#include "ErrorPacket.h"

class SynchPacket final :
	public CommandPacket
{
public:
	SynchPacket();
	virtual ~SynchPacket();
};

class SynchResponsePacket final : public ErrorPacket
{
	enum BytePositions
	{
		ERROR_CODE = 0x0,
	};
public:
	SynchResponsePacket();
	virtual ~SynchResponsePacket();
	static SynchResponsePacket* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize);
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

