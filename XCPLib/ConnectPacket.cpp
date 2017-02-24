#include "ConnectPacket.h"


ConnectPacket::ConnectPacket(ConnectMode cm) : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::CONNECT;
	m_DataLength = 1;
	m_Data = new uint8_t[m_DataLength];
	m_Data[0] = cm;
	m_PacketSize = 2;
}


ConnectPacket::~ConnectPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}
