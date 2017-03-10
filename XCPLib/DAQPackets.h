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

//--------------------------------------------------

class AllocOdtEntryPacket : public CommandPacket
{
	enum BytePositions
	{
		RESERVED = 0x00,
		DAQ_LIST_NUMBER = 0x01, //2bytes long!
		ODT_NUMBER = 0x03,
		ODT_ENTRIES_COUNT = 0x04
	};
public:
	AllocOdtEntryPacket();
	virtual ~AllocOdtEntryPacket();
	void SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian);
	void SetOdtNumber(uint8_t OdtNumber);
	void SetOdtEntriesCount(uint8_t OdtEntriesCount);
};

//--------------------------------------------------

class SetDaqPtrPacket : public CommandPacket
{
private:
	enum BytePositions
	{
		RESERVED = 0x00,
		DAQ_LIST_NUMBER = 0x01, //2 bytes long!
		ODT_NUMBER = 0x03,
		ODT_ENTRY_NUMBER = 0x04,
	};
public:
	SetDaqPtrPacket();
	virtual ~SetDaqPtrPacket();
	void SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian);
	void SetOdtNumber(uint8_t OdtNumber);
	void SetOdtEntryNumber(uint8_t OdtEntryNumber);
};