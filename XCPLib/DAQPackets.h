#pragma once
#include "CommandPacket.h"

class FreeDaqPacket : public CommandPacket
{
public:
	FreeDaqPacket();
	virtual ~FreeDaqPacket();
};

//--------------------------------------------------
class AllocDaqPacket : public CommandPacket
{
public:

};

