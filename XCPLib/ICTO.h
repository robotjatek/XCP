#pragma once
#include <cstdint>
#include "IXCPPacket.h"

enum CTOSlaveToMasterPacketTypes
{
	RES = 0xFF,
	ERR = 0xFE,
	EV = 0xFD,
	SERV = 0xFC,
};

enum CTOMasterToSlaveCommands
{
	//Standard Commands:
	CONNECT = 0xFF,
	DISCONNECT = 0xFE,
	GET_STATUS = 0xFD,
	SYNCH = 0xFC,
	GET_COMM_MODE_INFO = 0xFB,
	GET_ID = 0xFA,
	SET_REQUEST = 0xF9,
	GET_SEED = 0xF8,
	UNLOCK = 0xF7,
	SET_MTA = 0xF6,
	UPLOAD = 0xF5,
	SHORT_UPLOAD = 0xF4,
	BUILD_CHECKSUM = 0xF3,
	TRANSPORT_LAYER_CMD = 0xF2,
	USER_CMD = 0xF1,

	//Calibration commands:

	//Page switching commands:

	//Basic data acquisition and stimulation commands:
	SET_DAQ_PTR = 0xE2,
	WRITE_DAQ = 0xE1,
	SET_DAQ_LIST_MODE = 0xE0,
	START_STOP_DAQ_LIST = 0xDE,
	START_STOP_SYNCH = 0xDD,
	WRITE_DAQ_MULTIPLE = 0xC7,
	READ_DAQ = 0xDB,
	GET_DAQ_CLOCK = 0xDC,
	GET_DAQ_PROCESSOR_INFO = 0xDA,
	GET_DAQ_RESOLUTION_INFO = 0xD9,
	GET_DAQ_LIST_MODE = 0xDF,
	GET_DAQ_EVENT_INFO = 0xD7,
	DTO_CTR_PROPERTIES = 0xC5,

	//Static data acquisition and stim commands:
	CLEAR_DAQ_LIST = 0xE3,
	GET_DAQ_LIST_INFO = 0xD8,

	//Dynamic data acquisition and stim commands:
	FREE_DAQ = 0xD6,
	ALLOC_DAQ = 0xD5,
	ALLOC_ODT = 0xD4,
	ALLOC_ODT_ENTRY = 0xD3,

	//Non-volatile memory programming commands:

	//Time sync commands:
};

class ICTO : public IXCPPacket
{
public:
	ICTO() : IXCPPacket() {}
	virtual ~ICTO() {}
};