#include "DisconnectPacket.h"


DisconnectPacket::DisconnectPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::DISCONNECT;
	m_DataLength = 0;
	m_PacketSize = 1;
}


DisconnectPacket::~DisconnectPacket()
{
}