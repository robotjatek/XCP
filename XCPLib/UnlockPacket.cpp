#include "UnlockPacket.h"


UnlockPacket::UnlockPacket(const std::vector<uint8_t>& Key, uint8_t StartIndex, uint8_t MaxCto) : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::UNLOCK;
	if ((uint8_t)Key.size() - StartIndex >= MaxCto - 2)
	{
		m_DataLength = MaxCto - 2;
	}
	else
	{
		m_DataLength = (uint32_t)Key.size() + 1;
	}

	m_PacketSize = m_DataLength + 1;
	m_Data = new uint8_t[m_DataLength];
	m_Data[BytePositions::REMAINING_KEY_LENGTH] = (uint8_t)Key.size() - StartIndex;
	for (unsigned int i = 0; i < m_DataLength-1; i++)
	{
		m_Data[1 + i] = Key[StartIndex + i];
	}
}

UnlockPacket::~UnlockPacket()
{
	delete[] m_Data;
}

std::vector<IXCPPacket*> UnlockPacket::CreateUnlockPackets(const std::vector<uint8_t>& Key, uint8_t MaxCto)
{
	std::vector<IXCPPacket*> ret;
	uint8_t RemainingKeyLength = (uint8_t)Key.size();
	do
	{
		UnlockPacket* ToAdd = new UnlockPacket(Key, 0, MaxCto);
		RemainingKeyLength -= ToAdd->GetRemainingKeyLength();
		ret.push_back(ToAdd);
	} while (RemainingKeyLength);

	return ret;
}

uint8_t UnlockPacket::GetRemainingKeyLength()
{
	return m_Data[BytePositions::REMAINING_KEY_LENGTH];
}

UnlockResponsePacket::UnlockResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize)
{
	m_DataLength = 1;
	m_PacketSize = m_DataLength + 1;
	m_Data = new uint8_t[m_DataLength];
	m_Data[BytePositions::CURRENT_RESOURCE_PROTECTION_STATUS] = Data[HeaderSize + 1];
}

UnlockResponsePacket::~UnlockResponsePacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void UnlockResponsePacket::SetCurrentResourceProtection(uint8_t CurrentProection)
{
	m_Data[BytePositions::CURRENT_RESOURCE_PROTECTION_STATUS] = CurrentProection;
}

uint8_t UnlockResponsePacket::GetCurrentResourceProtection()
{
	return m_Data[BytePositions::CURRENT_RESOURCE_PROTECTION_STATUS];
}

void UnlockResponsePacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}

UnlockResponsePacket * UnlockResponsePacket::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize)
{
	return new UnlockResponsePacket(Data, HeaderSize, TailSize);
}
