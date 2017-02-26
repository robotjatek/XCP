#pragma once
#include <cstdint>
#include "ICTO.h"

class CommandPacket : public ICTO
{
public:
	CommandPacket() : ICTO() {};
	virtual ~CommandPacket() {}
};

