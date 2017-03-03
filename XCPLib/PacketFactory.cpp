#include "PacketFactory.h"
#include "ConnectPositivePacket.h"
#include "GetStatusPacket.h"
#include "SynchPacket.h"
#include "DisconnectPacket.h"
#include "SetMTAPacket.h"
#include "UploadPacket.h"
#include "DAQPackets.h"
#include "ErrorAccessLockedPacket.h"
#include "ErrorOutOfRangePacket.h"
#include "XCPMaster.h"
#include <iostream>


IXCPPacket * PacketFactory::CreateResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize, CommandPacket * LastSentCommand)
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
	case CTOMasterToSlaveCommands::UPLOAD:
		return UploadResponse::Deserialize(Data, HeaderSize, m_Master.GetSlaveProperties().AddressGranularity);
		break;
	case CTOMasterToSlaveCommands::SHORT_UPLOAD:
		return UploadResponse::Deserialize(Data, HeaderSize, m_Master.GetSlaveProperties().AddressGranularity);
		break;
	case CTOMasterToSlaveCommands::SET_MTA:
		return new ResponsePacket();
		break;
	default:
		std::cout << "Unhandled response format\n";
		return new ResponsePacket();
		break;
	}
}

IXCPPacket * PacketFactory::CreateErrorPacket(const std::vector<uint8_t>& data, uint8_t header_size, CommandPacket * LastSentCommand)
{
	uint8_t ErrorCode = data[header_size + 1];
	uint8_t LastCommandPID = LastSentCommand->GetPid();
	switch (ErrorCode)
	{
	case ErrorCodes::ERR_CMD_SYNCH:
		return SynchResponsePacket::Deserialize(data, header_size);
		break;
	case ErrorCodes::ERR_ACCESS_LOCKED:
		return new ErrorAccessLockedPacket();
		break;
	case ErrorCodes::ERR_OUT_OF_RANGE:
		return new ErrorOutOfRangePacket();
		break;
	default:
		std::cout << "Deserialization error: Unhandled errorcode\n";
		return nullptr;
		break;
	}
}

PacketFactory::PacketFactory(XCPMaster& master) : m_Master(master)
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

IXCPPacket * PacketFactory::CreateSynchPacket()
{
	return new SynchPacket();
}

IXCPPacket * PacketFactory::CreateSetMTAPacket(uint32_t address, uint8_t extension, bool LittleEndian)
{
	SetMTAPacket* packet = new SetMTAPacket();
	packet->SetAddress(address, LittleEndian);
	packet->SetAddressExtension(extension);
	return packet;
}

IXCPPacket * PacketFactory::CreateUploadPacket(uint8_t NumberOfElements)
{
	return new UploadPacket(NumberOfElements);
}

IXCPPacket * PacketFactory::CreateShortUploadPacket(uint8_t NumberOfElements, uint32_t Address, uint8_t AddressExtension, bool LittleEndian)
{
	ShortUploadPacket* packet = new ShortUploadPacket();
	packet->SetAddress(Address, LittleEndian);
	packet->SetAddressExtension(AddressExtension);
	packet->SetNumberOfDataElements(NumberOfElements);
	return packet;
}

IXCPPacket * PacketFactory::CreateFreeDaqPacket()
{
	return new FreeDaqPacket();
}

IXCPPacket * PacketFactory::CreateAllocDaqPacket(uint16_t DaqCount)
{
	AllocDaqPacket* packet = new AllocDaqPacket();
	packet->SetDaqCount(DaqCount, m_Master.GetSlaveProperties().ByteOrder == 0);
	return packet;
}

IXCPPacket * PacketFactory::DeserializeIncomingFromSlave(const std::vector<uint8_t>& Data, uint8_t HeaderSize, CommandPacket* LastSentCommand)
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
		return CreateErrorPacket(Data, HeaderSize, LastSentCommand);
		break;
	case CTOSlaveToMasterPacketTypes::SERV:
		break;
	default:
		break;
	}
	return nullptr;
}
