#pragma once
#include "CommandPacket.h"

enum ConnectMode
{
	NORMAL = 0x00,
	USER_DEFINED = 0x01
};

class ConnectPacket : public CommandPacket
{
public:
	ConnectPacket(ConnectMode cm);
	virtual ~ConnectPacket();
};

