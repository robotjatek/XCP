#pragma once
#include "CommandPacket.h"



class ConnectPacket final : public CommandPacket
{
public:
	enum ConnectMode
	{
		NORMAL = 0x00,
		USER_DEFINED = 0x01
	};
	ConnectPacket(ConnectMode cm);
	virtual ~ConnectPacket();
};

