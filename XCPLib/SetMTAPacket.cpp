#include "SetMTAPacket.h"



SetMTAPacket::SetMTAPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::SET_MTA;
	m_DataLength = 7;
	m_Data = new uint8_t[m_DataLength];
	m_PacketSize = 8;
}


SetMTAPacket::~SetMTAPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

uint8_t SetMTAPacket::GetAddressExtension()
{
	return m_Data[BytePositions::ADDRESS_EXTENSION];
}

void SetMTAPacket::SetAddressExtension(uint8_t AddressExtension)
{
	m_Data[BytePositions::ADDRESS_EXTENSION] = AddressExtension;
}

uint32_t SetMTAPacket::GetAddress(bool LittleEndian)
{
	if (LittleEndian)
	{
		return  (((uint32_t)m_Data[BytePositions::ADDRESS + 3]) << 24) | (((uint32_t)m_Data[BytePositions::ADDRESS + 2]) << 16) | (((uint32_t)m_Data[BytePositions::ADDRESS + 1]) << 8) | m_Data[BytePositions::ADDRESS];
	}
	else
	{
		//do byte-swap
		return (((uint32_t)m_Data[BytePositions::ADDRESS]) << 24) | (((uint32_t)m_Data[BytePositions::ADDRESS + 1]) << 16) | (((uint32_t)m_Data[BytePositions::ADDRESS + 2]) << 8) | m_Data[BytePositions::ADDRESS + 3];
	}
}

void SetMTAPacket::SetAddress(uint32_t Address, bool LittleEndian)
{
	uint8_t i1, i2, i3, i4;
	i1 = Address & 0xFF;
	i2 = (Address >> 8) & 0xFF;
	i3 = (Address >> 16) & 0xFF;
	i4 = (Address >> 24) & 0xFF;

	if (LittleEndian)
	{
		m_Data[BytePositions::ADDRESS] = i1;
		m_Data[BytePositions::ADDRESS + 1] = i2;
		m_Data[BytePositions::ADDRESS + 2] = i3;
		m_Data[BytePositions::ADDRESS + 3] = i4;
	}
	else
	{
		m_Data[BytePositions::ADDRESS] = i4;
		m_Data[BytePositions::ADDRESS + 1] = i3;
		m_Data[BytePositions::ADDRESS + 2] = i2;
		m_Data[BytePositions::ADDRESS + 3] = i1;
	}
}
