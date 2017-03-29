#pragma once
#include "ResponsePacket.h"
class ConnectPositivePacket final :
	public ResponsePacket
{
private:
	//These are the byte start positions in the m_Data member variable
	enum BytePositions
	{
		RESOURCE					= 0,
		COMM_MODE_BASIC				= 1,
		MAX_CTO						= 2,
		MAX_DTO						= 3, //2 bytes long!
		PROTOCOL_LAYER_VERSION		= 5,
		TRANSPORT_LAYER_VERSION		= 6,
	};

	ConnectPositivePacket(const std::vector<uint8_t>& data, uint8_t header_size);
public:
	//If a resource is available the mandatory commands of this resource must be supported
	enum ResourceParameterBits
	{
		CAL_PG	= 0x1,	//Calibration and Paging: 0 = not available , 1 = avilable
		DAQ		= 0x4,	//DAQ list supported: 0 = not avaliable, 1 = available
		STIM	= 0x8,	//STIM - Data stimulation of a daq list: 0 = not available, 1 = available
		PGM		= 0x10,	//Programming: 0 = flash programming not available, 1 = available
	};

	enum CommModeBasicBits
	{
		BYTE_ORDER				= 0x1,	//Byte order for multibyte parameters. 0 = Little endian (Intel format), 1 = Big Endian (Motorola format)
		ADDRESS_GRANULARITY_0	= 0x2,	//The address granularity indicates the size of an element 
		ADDRESS_GRANULARITY_1	= 0x4,	//The address granularity indicates the size of an element: 00-byte, 01-word, 10-DWORD, 11-reserved
		ADDRESS_GRANULARITY_BOTH = 0x6,
		SLAVE_BLOCK_MODE		= 0x40, //Inidicates if slave block mode is available
		OPTIONAL				= 0x80, //The OPTIONAL flag indicates whether additional information on supported types of 	Communication mode is available.The master can get that additional information with GET_COMM_MODE_INFO.
	};

	ConnectPositivePacket(uint8_t resource, uint8_t comm_mode_basic, uint8_t max_cto, uint16_t max_dto, uint8_t protocol_layer_version, uint8_t transport_layer_version);
	virtual ~ConnectPositivePacket();
	uint8_t GetResource();
	uint8_t GetCommModeBasic();
	uint8_t GetMaxCto();
	uint16_t GetMaxDto(bool LittleEndian);
	uint8_t GetProtocolLayerVersion();
	uint8_t GetTransportLayerVersion();
	void SetResource(uint8_t resource);
	void SetCommModeBasic(uint8_t comm_mode_basic);
	void SetMaxCto(uint8_t cto);
	void SetMaxDto(uint16_t dto);
	void SetProtocolLayerVersion(uint8_t version);
	void SetTransportLayerVersion(uint8_t version);
	static ConnectPositivePacket* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize);
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

