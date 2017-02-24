#include "IXCPMessage.h"

IXCPMessage::IXCPMessage()
{
	m_Header = nullptr;
	m_Packet = nullptr;
	m_Tail = nullptr;
}

IXCPMessage::~IXCPMessage()
{
	delete m_Header;
	delete m_Packet;
	delete m_Tail;
}

IXCPPacket * IXCPMessage::GetPacket()
{
	return m_Packet;
}

void IXCPMessage::SetPacket(IXCPPacket * packet)
{
	m_Packet = packet;
}

void IXCPMessage::Serialize(std::vector<uint8_t>& out_Binary)
{
	if (m_Header)
	{
		m_Header->Serialize(out_Binary);
	}

	if (m_Packet)
	{
		m_Packet->Serialize(out_Binary);
	}

	if (m_Tail)
	{
		m_Tail->Serialize(out_Binary);
	}
}

uint8_t IXCPMessage::GetHeaderSize()
{
	if (m_Header)
	{
		return m_Header->GetSize();
	}

	return 0;
}

uint8_t IXCPMessage::GetTailSize()
{
	if (m_Tail)
	{
		return m_Tail->GetSize();
	}

	return 0;
}
