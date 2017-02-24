#pragma once
#include <cstdint>
#include "ICTO.h"

class CommandPacket : public ICTO
{
public:
	CommandPacket() : ICTO() {};
	virtual ~CommandPacket() {}
	//virtual uint8_t* GetRawBytes() = 0;
};

