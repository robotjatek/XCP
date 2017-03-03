#include "DAQPackets.h"

FreeDaqPacket::FreeDaqPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::FREE_DAQ;
	m_Data = nullptr;
	m_DataLength = 0;
	m_PacketSize = 1;
}

FreeDaqPacket::~FreeDaqPacket()
{
}

//-----------------------------------------------------------------