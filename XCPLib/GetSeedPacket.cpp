#include "GetSeedPacket.h"
#include "ResponsePacket.h"


GetSeedPacket::GetSeedPacket() : CommandPacket()
{
	m_DataLength = 2;
	m_PacketSize = 3;
	m_Data = new uint8_t[m_DataLength];
	m_PID = CTOMasterToSlaveCommands::GET_SEED;
}

GetSeedPacket::GetSeedPacket(Mode Mode, Resource Resource) : GetSeedPacket()
{
	SetMode(Mode);
	SetResource(Resource);
}


GetSeedPacket::~GetSeedPacket()
{
	delete[] m_Data;
}

void GetSeedPacket::SetMode(Mode Mode)
{
	m_Data[BytePositions::MODE] = Mode;
}

void GetSeedPacket::SetResource(Resource Resource)
{
	m_Data[BytePositions::RESOURCE] = Resource;
}

uint8_t GetSeedPacket::GetResource()
{
	return m_Data[BytePositions::RESOURCE];
}

//--------------------------------------------------------

GetSeedResponsePacket::GetSeedResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize) : ResponsePacket()
{
	m_DataLength = (uint32_t)Data.size() - HeaderSize - TailSize - 1;
	m_PacketSize = m_DataLength + 1;
	m_Data = new uint8_t[m_DataLength];
	for (unsigned int i = 0; i < m_DataLength; i++)
	{
		m_Data[i] = Data[HeaderSize + 1 + i];
	}
}

GetSeedResponsePacket::~GetSeedResponsePacket()
{
	delete[] m_Data;
}

uint8_t GetSeedResponsePacket::GetLengthField()
{
	return m_Data[BytePositions::LENGTH];
}

uint8_t GetSeedResponsePacket::GetSeedPart(uint8_t Element)
{
	return m_Data[BytePositions::SEED+Element];
}

uint8_t GetSeedResponsePacket::GetSeedPartSize()
{
	return m_DataLength-1;
}

GetSeedResponsePacket * GetSeedResponsePacket::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize)
{
	return new GetSeedResponsePacket(Data,HeaderSize,TailSize);
}

void GetSeedResponsePacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
