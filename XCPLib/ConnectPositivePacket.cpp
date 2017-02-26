#include "ConnectPositivePacket.h"



ConnectPositivePacket::ConnectPositivePacket(uint8_t resource, uint8_t comm_mode_basic, uint8_t max_cto, uint16_t max_dto, uint8_t protocol_layer_version, uint8_t transport_layer_version)
	: ResponsePacket::ResponsePacket()
{
	m_PacketSize = 8;
	m_DataLength = 7;
	m_Data = new uint8_t[m_DataLength];
	SetResource(resource);
	SetCommModeBasic(comm_mode_basic);
	SetMaxCto(max_cto);
	SetMaxDto(max_dto);
	SetProtocolLayerVersion(protocol_layer_version);
	SetTransportLayerVersion(transport_layer_version);
}

ConnectPositivePacket::ConnectPositivePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize)
	: ResponsePacket::ResponsePacket()
{
	m_PacketSize = 8;
	m_DataLength = 7;
	m_Data = new uint8_t[m_DataLength];

	//if (Data.size() == m_PacketSize + HeaderSize)
	{
		for (unsigned int i = 0; i < m_DataLength; i++)
		{
			m_Data[i] = Data[HeaderSize + i + 1]; //Here the Data vector also stores the TCP packet header (first 4 bytes) and the PID value (1 byte), so the offset of the real parameters start from the sixth byte (in case of TCP packets)
		}
	}
	//	else
	{
		//This should not happen at all
//		throw 666;
	}
}

ConnectPositivePacket::~ConnectPositivePacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

uint8_t ConnectPositivePacket::GetResource()
{
	return m_Data[BytePositions::RESOURCE];
}

uint8_t ConnectPositivePacket::GetCommModeBasic()
{
	return m_Data[BytePositions::COMM_MODE_BASIC];
}

uint8_t ConnectPositivePacket::GetMaxCto()
{
	return m_Data[BytePositions::MAX_CTO];
}

uint16_t ConnectPositivePacket::GetMaxDto(bool LittleEndian)
{
	if (LittleEndian)
	{
		return (((uint16_t)m_Data[BytePositions::MAX_DTO + 1]) << 8) | m_Data[BytePositions::MAX_DTO];
	}
	else
	{
		//do byte-swap
		return (((uint16_t)m_Data[BytePositions::MAX_DTO]) << 8) | m_Data[BytePositions::MAX_DTO + 1];
	}
}

uint8_t ConnectPositivePacket::GetProtocolLayerVersion()
{
	return m_Data[BytePositions::PROTOCOL_LAYER_VERSION];
}

uint8_t ConnectPositivePacket::GetTransportLayerVersion()
{
	return m_Data[BytePositions::TRANSPORT_LAYER_VERSION];
}

void ConnectPositivePacket::SetResource(uint8_t resource)
{
	m_Data[BytePositions::RESOURCE] = resource;
}

void ConnectPositivePacket::SetCommModeBasic(uint8_t comm_mode_basic)
{
	m_Data[BytePositions::COMM_MODE_BASIC] = comm_mode_basic;
}

void ConnectPositivePacket::SetMaxCto(uint8_t cto)
{
	m_Data[BytePositions::MAX_CTO] = cto;
}

void ConnectPositivePacket::SetMaxDto(uint16_t dto)
{
	m_Data[BytePositions::MAX_DTO] = dto & 0xFF;
	m_Data[BytePositions::MAX_DTO + 1] = (dto & 0xFF00) >> 8;
}

void ConnectPositivePacket::SetProtocolLayerVersion(uint8_t version)
{
	m_Data[BytePositions::PROTOCOL_LAYER_VERSION] = version;
}

void ConnectPositivePacket::SetTransportLayerVersion(uint8_t version)
{
	m_Data[BytePositions::TRANSPORT_LAYER_VERSION] = version;
}

ConnectPositivePacket * ConnectPositivePacket::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize)
{
	return new ConnectPositivePacket(Data, HeaderSize);
}

void ConnectPositivePacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
