#pragma once
#include "CommandPacket.h"
#include "ResponsePacket.h"

class GetStatusPacket final :
	public CommandPacket
{
public:
	GetStatusPacket();
	virtual ~GetStatusPacket();
};

class GetStatusResponsePacket final : public ResponsePacket
{
	enum BytePositions //Byte offsets in m_Data
	{
		CURRENT_SESSION_STATUS = 0x00,
		CURRENT_RESOURCE_PROTECTION_STATUS = 0x01,
		STATE_NUMBER = 0x02,
		SESSION_CONFIGURATION_ID = 0x03, //2 bytes long!
	};
public:
	enum CurrentSessionStatusBits
	{
		STORE_CAL_REQ = 0x01, //Pending request to store data into non-volatile memory
		STORE_DAQ_REQ = 0x04, //Pending request to save daq list into non-volatile memory
		CLEAR_DAQ_REQ = 0x08, //pending request to clear all daq list in non-volatile memory
		DAQ_RUNNING = 0x40, //at least one daq list has been started and is in running mode
		RESUME = 0x80, //slave is in resume mode
	};

	enum CurrentResourceProtectionBits //The given resorce is protected with Seed&key. If a resource is protected, an attempt to exectue a command on it before a successful GET_SEED/UNLOCK sequence will result in ERR_ACCESS_LOCKED 
	{
		CAL_PG = 0x01,
		DAQ = 0x04,
		STIM = 0x08,
		PGM = 0x10,
	};

	GetStatusResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize);
	virtual ~GetStatusResponsePacket();
	static GetStatusResponsePacket* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize);
	virtual void Dispatch(IIncomingMessageHandler& Handler);
	uint8_t GetCurrentSessionStatus();
	void SetCurrentSessionStatus(uint8_t SessionStatus);
	uint8_t GetCurrentResourceProtection();
	void SetCurrentResourceProtection(uint8_t ResourceProtection);
	uint8_t GetStateNumber(); //If the XCP slave supports ECU_STATES the current STATE_NUMBER will be given to the XCP master in the response of GET_STATUS.
	void SetStateNumber(uint8_t State);
	uint16_t GetSessionConfigurationId(bool LittleEndian);
	void SetSessionConfigurationId(uint16_t SessionConfigurationId);
};

