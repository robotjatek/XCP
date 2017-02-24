#include "TCPHeader.h"

TCPHeader::TCPHeader(uint16_t len, uint16_t ctr)
{
	m_Length = len;
	m_Ctr = ctr;
}

TCPHeader::~TCPHeader()
{
}

void TCPHeader::SetLength(uint16_t len)
{
	m_Length = len;
}

uint16_t TCPHeader::GetLength()
{
	return m_Length;
}

void TCPHeader::SetCtr(uint16_t ctr)
{
	m_Ctr = ctr;
}

uint16_t TCPHeader::GetCtr()
{
	return m_Ctr;
}

void TCPHeader::Serialize(std::vector<uint8_t>& out_Binary)
{
	out_Binary.push_back((uint8_t)(m_Length) & 0xFF);
	out_Binary.push_back((uint8_t)(((m_Length) & 0xFF00)>>8));
	out_Binary.push_back((uint8_t)(m_Ctr) & 0xFF);
	out_Binary.push_back((uint8_t)(((m_Ctr) & 0xFF00) >> 8));
}

uint8_t TCPHeader::GetSize()
{
	return 4;
}
