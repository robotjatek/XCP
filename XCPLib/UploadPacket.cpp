#include "UploadPacket.h"



UploadPacket::UploadPacket(uint8_t NumberOfDataElements) : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::UPLOAD;
	m_PacketSize = 2;
	m_DataLength = 1;
	m_Data = new uint8_t[m_DataLength];
	m_Data[0] = NumberOfDataElements;
}


UploadPacket::~UploadPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void UploadPacket::SetNumberOfDataElements(uint8_t num)
{
	m_Data[0] = num;
}

uint8_t UploadPacket::GetNumberOfDataElements()
{
	return m_Data[0];
}

//----------------------------------------------------------------------------------------------------

UploadResponse::UploadResponse(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t AG) : ResponsePacket()
{
	m_DataLength = (uint32_t)Data.size() - HeaderSize - 1; //size -headersize - PID fieldsize
	m_PacketSize = m_DataLength + 1;
	m_Data = new uint8_t[m_DataLength];
	for (unsigned int i = 0; i < m_DataLength; i++)
	{
		m_Data[i] = Data[HeaderSize + 1 + i];
	}
	m_AG = AG;
	m_ElementSizeInBytes = 1 << AG;
	if (m_ElementSizeInBytes == 1)
	{
		m_NumberOfElements = m_DataLength;
	}
	else if (m_ElementSizeInBytes == 2)
	{
		m_NumberOfElements = (m_DataLength - 1)/2;
	}
	else if (m_ElementSizeInBytes == 4)
	{
		m_NumberOfElements = (m_DataLength - 3)/4;
	}
	else //ERROR
	{
		m_NumberOfElements = 0;
	}
}

UploadResponse::~UploadResponse()
{
	delete[] m_Data;
	m_Data = nullptr;
}

UploadResponse * UploadResponse::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t AG)
{
	return new UploadResponse(Data, HeaderSize, AG);
}

uint8_t UploadResponse::GetNumberOfElements()
{
	
	return m_NumberOfElements;
}

void UploadResponse::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}

/*
template<uint8_t>
uint8_t UploadResponse::GetElement(uint8_t id, bool LittleEndian)
{
	return m_Data[id];
}

template<uint16_t>
uint16_t UploadResponse::GetElement(uint8_t id, bool LittleEndian)
{
	uint16_t t1, t2;
	t1 = m_Data[1 + id * 2];
	t2 = m_Data[1 + id * 2 + 1];

	if (LittleEndian)
	{
		return ((t2) << 8) | t1;
	}
	else
	{
		//do byte-swap
		return ((t1) << 8) | t2;
	}
}

template<uint32_t>
uint32_t UploadResponse::GetElement(uint8_t id, bool LittleEndian)
{
	if (LittleEndian)
	{
		return  (((uint32_t)m_Data[3 + id * 4 + 3]) << 24) | (((uint32_t)m_Data[3 + id * 4 + 2]) << 16) | (((uint32_t)m_Data[3 + id * 4 + 1]) << 8) | m_Data[3 + id * 4];
	}
	else
	{
		//do byte-swap
		return (((uint32_t)m_Data[3 + id * 4]) << 24) | (((uint32_t)m_Data[3 + id * 4 + 1]) << 16) | (((uint32_t)m_Data[3 + id * 4 + 2]) << 8) | m_Data[3 + id * 4 + 3];
	}
}
*/

//------------------------------------------------------------------------------------------------------------------

ShortUploadPacket::ShortUploadPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::SHORT_UPLOAD;
	m_DataLength = 7;
	m_PacketSize = 8;
	m_Data = new uint8_t[m_DataLength];
	m_Data[BytePositions::RESERVED] = 0;
}

ShortUploadPacket::~ShortUploadPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

uint32_t ShortUploadPacket::GetAddress(bool LittleEndian)
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

void ShortUploadPacket::SetAddress(uint32_t Address, bool LittleEndian)
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

uint8_t ShortUploadPacket::GetAddressExtension()
{
	return m_Data[BytePositions::ADDRESS_EXTENSION];
}

void ShortUploadPacket::SetAddressExtension(uint8_t AddressExtension)
{
	m_Data[BytePositions::ADDRESS_EXTENSION] = AddressExtension;
}

void ShortUploadPacket::SetNumberOfDataElements(uint8_t NumberOfDataElements)
{
	m_Data[BytePositions::NUMBER_OF_DATA_ELEMENTS] = NumberOfDataElements;
}

uint8_t ShortUploadPacket::GetNumberOfDataElements()
{
	return m_Data[BytePositions::NUMBER_OF_DATA_ELEMENTS];
}

