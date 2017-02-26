#include "GetStatusPacket.h"



GetStatusPacket::GetStatusPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::GET_STATUS;
	m_DataLength = 0;
	m_PacketSize = 1;
}


GetStatusPacket::~GetStatusPacket()
{
}

//-------------------------------------------------------------------------------------------------------------------------------

GetStatusResponsePacket::GetStatusResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize) : ResponsePacket()
{
	m_DataLength = 5;
	m_PacketSize = 6;

	m_Data = new uint8_t[m_DataLength];
	for (unsigned int i = 0; i < m_DataLength; i++)
	{
		m_Data[i] = Data[HeaderSize + i + 1];
	}
}

GetStatusResponsePacket::~GetStatusResponsePacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

GetStatusResponsePacket * GetStatusResponsePacket::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize)
{
	return new GetStatusResponsePacket(Data,HeaderSize);
}

void GetStatusResponsePacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}

uint8_t GetStatusResponsePacket::GetCurrentSessionStatus()
{
	return m_Data[BytePositions::CURRENT_SESSION_STATUS];
}

void GetStatusResponsePacket::SetCurrentSessionStatus(uint8_t SessionStatus)
{
	m_Data[BytePositions::CURRENT_SESSION_STATUS] = SessionStatus;
}

uint8_t GetStatusResponsePacket::GetCurrentResourceProtection()
{
	return m_Data[BytePositions::CURRENT_RESOURCE_PROTECTION_STATUS];
}

void GetStatusResponsePacket::SetCurrentResourceProtection(uint8_t ResourceProtection)
{
	m_Data[BytePositions::CURRENT_RESOURCE_PROTECTION_STATUS] = ResourceProtection;
}

uint8_t GetStatusResponsePacket::GetStateNumber()
{
	return m_Data[BytePositions::STATE_NUMBER];
}

void GetStatusResponsePacket::SetStateNumber(uint8_t State)
{
	m_Data[BytePositions::STATE_NUMBER] = State;
}

uint16_t GetStatusResponsePacket::GetSessionConfigurationId(bool LittleEndian)
{
	if (LittleEndian)
	{
		return (((uint16_t)m_Data[BytePositions::SESSION_CONFIGURATION_ID + 1]) << 8) | m_Data[BytePositions::SESSION_CONFIGURATION_ID];
	}
	else
	{
		//do byte-swap
		return (((uint16_t)m_Data[BytePositions::SESSION_CONFIGURATION_ID]) << 8) | m_Data[BytePositions::SESSION_CONFIGURATION_ID + 1];
	}
}

void GetStatusResponsePacket::SetSessionConfigurationId(uint16_t SessionConfigurationId)
{
	m_Data[BytePositions::SESSION_CONFIGURATION_ID] = SessionConfigurationId & 0xFF;
	m_Data[BytePositions::SESSION_CONFIGURATION_ID + 1] = (SessionConfigurationId & 0xFF00) >> 8;
}
