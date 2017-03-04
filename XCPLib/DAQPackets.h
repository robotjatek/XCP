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
		RESERVED = 0x00,
		DAQ_COUNT = 0x01, //2 bytes!
	};
public:
	AllocDaqPacket();
	virtual ~AllocDaqPacket();
	void SetDaqCount(uint16_t DaqCount, bool LittleEndian);
	uint16_t GetDaqCount(bool LittleEndian);
};

//--------------------------------------------------

class AllocOdtPacket : public CommandPacket
{
	enum BytePositions
	{
		RESERVED = 0x00,
		DAQ_LIST_NUMBER = 0x01, //2bytes
		ODT_COUNT = 0x03,
	};
public:
	AllocOdtPacket();
	virtual ~AllocOdtPacket();
	void SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian);
	void SetOdtCount(uint8_t OdtCount);
};

