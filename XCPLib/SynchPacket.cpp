#include "SynchPacket.h"



SynchPacket::SynchPacket() :CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::SYNCH;
	m_PacketSize = 1;
	m_DataLength = 0;
	m_Data = nullptr;
}


SynchPacket::~SynchPacket()
{
}

SynchResponsePacket::SynchResponsePacket() : ErrorPacket()
{
	m_PacketSize = 2;
	m_DataLength = 1;
	m_Data = new uint8_t[m_DataLength];
	m_Data[BytePositions::ERROR_CODE] = ErrorCodes::ERR_CMD_SYNCH;
}

SynchResponsePacket::~SynchResponsePacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

SynchResponsePacket * SynchResponsePacket::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize)
{
	return new SynchResponsePacket();
}

void SynchResponsePacket::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}
