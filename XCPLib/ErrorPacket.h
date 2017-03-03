#pragma once
#include "ICTO.h"

enum ErrorCodes
{
	ERR_CMD_SYNCH = 0x00,
	ERR_CMD_BUSY = 0x10,
	ERR_DAQ_ACTIVE = 0x11,
	ERR_PGM_ACTIVE = 0x12,
	ERR_CMD_UNKNOWN = 0x20,
	ERR_CMD_SYNTAX = 0x21,
	ERR_OUT_OF_RANGE = 0x22, 
	ERR_WRITE_PROTECTED = 0x23,
	ERR_ACCESS_DENIED = 0x24,
	ERR_ACCESS_LOCKED = 0x25,
	ERR_PAGE_NOT_VALID = 0x26,
	ERR_MODE_NOT_VALID = 0x27,
	ERR_SEGMENT_NOT_VALID = 0x28,
	ERR_SEQUENCE = 0x29,
	ERR_DAQ_CONFIG = 0x2A,
	ERR_MEMORY_OVERFLOW = 0x30,
	ERR_GENERIC = 0x31,
	ERR_VERIFY = 0x32,
	ERR_RESOURCE_TEMPORARY_NOT_ACCESSIBLE = 0x33,
	ERR_SUBCMD_UNKNOWN = 0x34,
};

class ErrorPacket : public ICTO
{
public:
	ErrorPacket() : ICTO()
	{
		m_PID = CTOSlaveToMasterPacketTypes::ERR;
		m_Data = nullptr;
	}

	virtual void SetErrorCode(uint8_t ErrorCode)
	{
		m_Data[0] = ErrorCode;
	}

	virtual uint8_t GetErrorCode()
	{
		return m_Data[0];
	}
};