#include "TCPMessage.h"
#include "TCPHeader.h"

TCPMessage::TCPMessage(IXCPPacket * packet, uint16_t ctr) : IXCPMessage()
{
	m_Packet = packet;
	m_Header = new TCPHeader(packet->GetPacketSize(), ctr);
	m_Tail = nullptr; //There is no tail for the TCP message format
}

TCPMessage::~TCPMessage()
{
	//Deletion is done in the parent class
}
