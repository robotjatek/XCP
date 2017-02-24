#pragma once
#include "CommandPacket.h"
class DisconnectPacket : public CommandPacket
{
public:
	DisconnectPacket();
	virtual ~DisconnectPacket();
};

