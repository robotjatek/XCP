#include "IXCPPacket.h"

void IXCPPacket::Serialize(std::vector<uint8_t>& out_Binary)
{
	out_Binary.push_back(m_PID);
	for (unsigned int i = 0; i < m_DataLength; i++)
	{
		out_Binary.push_back(m_Data[i]);
	}
}

uint32_t IXCPPacket::GetDataLength()
{
	return m_DataLength;
}

uint32_t IXCPPacket::GetPacketSize()
{
	return m_PacketSize;
}

IXCPPacket::~IXCPPacket()
{
}

uint8_t IXCPPacket::GetPid()
{
	return m_PID;
}
