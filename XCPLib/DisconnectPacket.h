#pragma once
#include "CommandPacket.h"
class DisconnectPacket final : public CommandPacket
{
public:
	DisconnectPacket();
	virtual ~DisconnectPacket();
};

