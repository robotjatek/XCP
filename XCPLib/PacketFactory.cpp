#include "PacketFactory.h"
#include "ConnectPositivePacket.h"
#include "GetStatusPacket.h"
#include <iostream>


IXCPPacket * PacketFactory::CreateResponsePacket(std::vector<uint8_t>& Data, uint8_t HeaderSize, CommandPacket * LastSentCommand)
{
	uint8_t LastCommandPID = LastSentCommand->GetPid();
	switch (LastCommandPID)
	{
	case CTOMasterToSlaveCommands::CONNECT:
		return ConnectPositivePacket::Deserialize(Data, HeaderSize);
		break;
	case CTOMasterToSlaveCommands::DISCONNECT:
		return new ResponsePacket();
		break;
	case CTOMasterToSlaveCommands::GET_STATUS:
		return GetStatusResponsePacket::Deserialize(Data, HeaderSize);
		break;
	default:
		std::cout << "Unhandled response format\n";
		return nullptr;
		break;
	}
	//	return nullptr;
}

PacketFactory::PacketFactory()
{
}


PacketFactory::~PacketFactory()
{
}

IXCPPacket * PacketFactory::CreateConnectPacket(ConnectMode mode)
{
	return new ConnectPacket(mode);
}

IXCPPacket * PacketFactory::CreateDisconnectPacket()
{
	return new DisconnectPacket();
}

IXCPPacket * PacketFactory::CreateGetStatusPacket()
{
	return new GetStatusPacket();
}

IXCPPacket * PacketFactory::DeserializeIncomingFromSlave(std::vector<uint8_t>& Data, uint8_t HeaderSize, CommandPacket* LastSentCommand)
{
	uint8_t PID = Data[HeaderSize];
	switch (PID)
	{
	case CTOSlaveToMasterPacketTypes::RES:
		return CreateResponsePacket(Data, HeaderSize, LastSentCommand);
		break;
	case CTOSlaveToMasterPacketTypes::EV:
		break;
	case CTOSlaveToMasterPacketTypes::ERR:
		break;
	case CTOSlaveToMasterPacketTypes::SERV:
		break;
	default:
		break;
	}
	return nullptr;
}
