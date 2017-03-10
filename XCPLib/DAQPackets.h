#pragma once
#include "CommandPacket.h"
#include "ResponsePacket.h"

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

//--------------------------------------------------

class WriteDaqPacket : public CommandPacket
{
private:
	enum BytePositions
	{
		BIT_OFFSET = 0x00,
		SIZE = 0x01,
		ADDRESS_EXTENSION = 0x02,
		ADDRESS = 0x03, //4 bytes long!
	};
public:
	WriteDaqPacket();
	virtual ~WriteDaqPacket();
	//For DAQ diresction it is a bitmask, for STIM direction it is the position of the bit to be manipulated. The DAQ pointer is auto post incremented within an ODT. After the last ODT entry the pointer value is undefined.
	void SetBitOffset(uint8_t Offset);
	void SetElementSize(uint8_t Size);
	void SetAddressExtension(uint8_t AddressExtension);
	void SetAddress(uint32_t Address, bool LittleEndian);
};

//--------------------------------------------------

class SetDaqListModePacket : public CommandPacket
{
public:
	enum ModeFieldBits
	{
		ALTERNATING = 0x01, //0: disable alternating display mode, 1: enable
		DIRECTION = 0x02, //0: DAQ direction, 1: STIM direction
		DTO_CTR = 0x08, //0: do not use DTO ctr field, 1: use ctr
		TIMESTAMP = 0x10, //0: disable timestamp, 1: enable
		PID_OFF = 0x20, //0: transmit DTO with identification field, 1: transmit DTO WITHOUT identifictation field
	};
private:
	enum BytePositions
	{
		MODE = 0x00,
		DAQ_LIST_NUMBER = 0x01, //2 bytes long!
		EVENT_CHANNEL_NUMBER = 0x03, //2 bytes long!
		TRANSMISSION_RATE_PRESCALER = 0x05,
		DAQ_LIST_PRIORITY = 0x06,
	};
public:
	SetDaqListModePacket();
	SetDaqListModePacket(uint8_t Mode, uint16_t DaqListNumber, uint16_t EventChannel, uint8_t Prescaler, uint8_t Priority, bool LittleEndian);
	virtual ~SetDaqListModePacket();
	void SetMode(uint8_t Mode);
	void SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian);
	void SetEventChannelNumber(uint16_t EventChannel, bool LittleEndian);
	void SetTransmissionRatePrescaler(uint8_t Prescaler);
	void SetDaqListPriority(uint8_t Priority);
};

//--------------------------------------------------

class StartStopDaqListPacket : public CommandPacket
{
public:
	enum Mode
	{
		STOP = 0x0,
		START = 0x1,
		SELECT = 0x2,
	};
private:
	enum BytePositions
	{
		MODE = 0x00,
		DAQ_LIST_NUMBER = 0x01, //2 bytes long!
	};
public:
	StartStopDaqListPacket();
	//Mode: 0 stop, 1 start, 2 select
	StartStopDaqListPacket(uint8_t Mode, uint16_t DaqListNumber, bool LittleEndian);
	virtual ~StartStopDaqListPacket();
	void SetMode(uint8_t Mode);
	void SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian);
};

//--------------------------------------------------

class StartStopDaqListPositiveResponse : public ResponsePacket
{
	enum BytePositions
	{
		FIRST_PID = 0x0,
	};
public:
	StartStopDaqListPositiveResponse(const std::vector<uint8_t>& Data, uint8_t HeaderSize);
	virtual ~StartStopDaqListPositiveResponse();
	uint8_t GetFirstPid();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
	static StartStopDaqListPositiveResponse* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize);
};