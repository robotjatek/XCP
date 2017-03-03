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
	enum BytePositions
	{
		RESERVED = 0x0,
		DAQ_COUNT = 0x1, //2 bytes!
	};
public:
	AllocDaqPacket();
	virtual ~AllocDaqPacket();
	void SetDaqCount(uint16_t DaqCount, bool LittleEndian);
	uint16_t GetDaqCount(bool LittleEndian);
};

