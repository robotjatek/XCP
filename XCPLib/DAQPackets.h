#pragma once
#include "CommandPacket.h"
#include "ResponsePacket.h"

class FreeDaqPacket final : public CommandPacket
{
public:
	FreeDaqPacket();
	virtual ~FreeDaqPacket();
};

//--------------------------------------------------

class AllocDaqPacket final : public CommandPacket
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

class AllocOdtPacket final : public CommandPacket
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

class AllocOdtEntryPacket final : public CommandPacket
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

class SetDaqPtrPacket final : public CommandPacket
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

class WriteDaqPacket final : public CommandPacket
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

class SetDaqListModePacket final : public CommandPacket
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

class StartStopDaqListPacket final : public CommandPacket
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
	uint16_t GetDaqListNumber(bool LittleEndian);
};

//--------------------------------------------------

class StartStopDaqListPositiveResponse final : public ResponsePacket
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

//--------------------------------------------------

class StartStopSynchPacket final : public CommandPacket
{
private:
	enum BytePositions
	{
		MODE = 0x00,
	};
public:
	enum Mode
	{
		STOP_ALL = 0x00,
		START_SELECTED = 0x01,
		STOP_SELECTED = 0x02,
	};
	StartStopSynchPacket(Mode Mode);
	virtual ~StartStopSynchPacket();
	void SetMode(uint8_t Mode);
};

//--------------------------------------------------

class GetDaqProcessorInfo final : public CommandPacket
{
public:
	GetDaqProcessorInfo();
	virtual ~GetDaqProcessorInfo();
};

//--------------------------------------------------

class GetDaqProcessorInfoResponse final : public ResponsePacket
{
private:
	enum BytePositions
	{
		DAQ_PROPERTIES = 0x0,
		MAX_DAQ = 0x1, //2 bytes!
		MAX_EVENT_CHANNEL = 0x3, //2 bytes!
		MIN_DAQ = 0x5,
		DAQ_KEY_BYTE = 0x6,
	};
public:
	enum DaqPropertiesBits
	{
		DAQ_CONFIG_TYPE = 0x01, //0: Static DAQ list configuration, 1: dynamic daq list configuration
		PRESCALER_SUPPORTED = 0x02, //0: prescaler not supported, 1: prescaler supported
		RESUME_SUPPORTED = 0x04, //0: DAQ list can not be set to resume mode, 1: Daq list can be set to resume mode
		BIT_STIM_SUPPORTED = 0x08, //0: bitwise datastimulation not supported, 1: supported
		TIMESTAMP_SUPPORTED = 0x10, //0: timestamped mode not supported, 1: supported
		PID_OFF_SUPPORTED = 0x20, //0: Identification field can not be switched off, 1: Identification field may be switched off
		OVERLOAD_MSB = 0x40,
		OVERLOAD_EVENT = 0x80, //OE-OM: 0-0:No overload indication | 0-1:overload indication in MSB of PID | 1-0: overload indication by Event Packet | 1-1: not allowed

		OVERLOAD_INDICATION_MODE = 0xC0, //the 2 before, only combined
	};

	enum DaqKeyByteBits
	{
		OPTIMISATION_TYPE_0 = 0x01, //The Optimisation_Type flags indicate the type of Optimisation Method the master preferably should use.
		OPTIMISATION_TYPE_1 = 0x02, 
		OPTIMISATION_TYPE_2 = 0x04, 
		OPTIMISATION_TYPE_3 = 0x08, //OT3-2-1-0: 0000: OM_DEFAULT | 0001: OM_ODT_TYPE_16 | 0010: OM_ODT_TYPE_32 | 0011: OM_ODT_TYPE_64 | 0100: OM_ODT_TYPE_ALIGNMENT | 0101: OM_MAX_ENTRY_SIZE

		OPTIMISATION_TYPE = 0x0F, ////the 4 entries before this one, only that it is combined into one

		ADDRESS_EXTENSION_ODT = 0x10, //The ADDR_EXTENSION flag indicates whether the address extension of all entries within one ODT or within one DAQ must be the same.
		ADDRESS_EXTENSION_DAQ = 0x20, //AEDAQ-AEODT: 00: address extension can be different within one and the same ODT | 01: 0 1 address extension to be the same for all entries within one ODT | 10: not allowed | 11: address extension to be the same for all entries within one DAQ

		ADDRESS_EXTENSION = 0x30,

		IDENTIFICATION_FIELD_TYPE_0 = 0x40,
		IDENTIFICATION_FIELD_TYPE_1 = 0x80, //00: absoulute odt number | 01: Relative ODT number, absolute DAQ list number (BYTE) | Relative ODT number, absolute DAQ list number (WORD) | 11: Relative ODT number, absolute DAQ list number (WORD, aligned)

		IDENTIFICATION_FIELD_TYPE = 0xC0, //the two entries before this one, only that it is combined into one
	};

	enum OverloadIndicationMode
	{
		NO_INDICATION = 0x00,
		MSB_PID = 0x40,
		EVENT_PACKET = 0x80,
		NOT_ALLOWED = 0xC0,
	};

	enum OptimisationMode
	{
		OM_DEFAULT = 0x00,
		OM_ODT_TYPE_16 = 0x01,
		OM_ODT_TYPE_32 = 0x02,
		OM_ODT_TYPE_64 = 0x03,
		OM_ODT_TYPE_ALIGNMENT = 0x04,
		OM_MAX_ENTRY_SIZE = 0x05,
	};

	enum AddressExtensionType
	{
		CAN_BE_DIFFERENT = 0x00,
		SAME_FOR_ALL_ENTRIES_ODT = 0x10,
//		NOT_ALLOWED = 0x20,
		SAME_FOR_ALL_ENTRIES_DAQ = 0x30
	};

	enum IdentificationFieldType
	{
		ABSOLUTE_ODT_NUMBER = 0x00,
		RELATIVE_ODT_ABSOLUTE_DAQ_BYTE = 0x40,
		RELATIVE_ODT_ABSOLUTE_DAQ_WORD = 0x80,
		RELATIVE_ODT_ABSOLUTE_DAQ_WORD_ALIGNED = 0xC0,
	};

	GetDaqProcessorInfoResponse(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize);
	virtual ~GetDaqProcessorInfoResponse();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
	static GetDaqProcessorInfoResponse* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize);
	uint8_t GetDaqProperties();
	//total number of DAQ lists available in the slave device. If DAQ_CONFIG_TYPE =	dynamic, MAX_DAQ equals MIN_DAQ + DAQ_COUNT
	uint16_t GetMaxDaq(bool LittleEndian);
	//MAX_EVENT_CHANNEL is the number of available event channels. MAX_EVENT_CHANNEL = 0x00 means that the number of events in the slave is unknown.
	uint16_t GetMaxEventChannel(bool LittleEndian);
	//MIN_DAQ is the number of predefined DAQ lists.
	uint8_t GetMinDaq();
	uint8_t GetDaqKeyByte();
};

//--------------------------------------------------

class ClearDAQListPacket final : public CommandPacket
{
private:
	enum BytePositions
	{
		RESERVED = 0x00,
		DAQ_LIST_NUMBER = 0x01,
	};
public:
	ClearDAQListPacket(uint16_t DAQListNumber, bool LittleEndian);
	virtual ~ClearDAQListPacket();
	void SetDaqListNumber(uint16_t DAQListNumber, bool LittleEndian);
	uint16_t GetDaqListNumbet(bool LittleEndian);
};