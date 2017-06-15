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
#include "ErrorSequencePacket.h"
#include "ErrorMemoryOverflowPacket.h"
#include "GetSeedPacket.h"
#include "XCPMaster.h"
#include "UnlockPacket.h"
#include "DTO.h"
#include "ErrorCommandUnknown.h"
#include <iostream>


IXCPPacket * PacketFactory::CreateResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize, CommandPacket * LastSentCommand)
{
	if (LastSentCommand)
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
		case CTOMasterToSlaveCommands::START_STOP_DAQ_LIST:
			return StartStopDaqListPositiveResponse::Deserialize(Data, HeaderSize);
			break;
		case CTOMasterToSlaveCommands::GET_SEED:
			return GetSeedResponsePacket::Deserialize(Data, HeaderSize, TailSize);
			break;
		case CTOMasterToSlaveCommands::UNLOCK:
			return UnlockResponsePacket::Deserialize(Data, HeaderSize, TailSize);
			break;
		case CTOMasterToSlaveCommands::GET_DAQ_PROCESSOR_INFO:
			return GetDaqProcessorInfoResponse::Deserialize(Data, HeaderSize, TailSize);
			break;
		default:
			return new ResponsePacket();
			break;
		}
	}
	std::cout << "Internal error: Last sent command is a nullptr\a\n";
	return nullptr;
}

IXCPPacket * PacketFactory::CreateErrorPacket(const std::vector<uint8_t>& data, uint8_t header_size, uint8_t TailSize, CommandPacket * LastSentCommand)
{
	if (LastSentCommand)
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
		case ErrorCodes::ERR_SEQUENCE:
			return new ErrorSequencePacket();
			break;
		case ErrorCodes::ERR_MEMORY_OVERFLOW:
			return new ErrorMemoryOverflowPacket();
			break;
		case ErrorCodes::ERR_CMD_UNKNOWN:
			return new ErrorCommandUnknown();
			break;
		default:
			std::cout << "Deserialization error: Unhandled errorcode\n";
			return nullptr;
			break;
		}
	}
	std::cout << "Internal error: Last sent command is a nullptr\a\n";
	return nullptr;
}

IXCPPacket * PacketFactory::DeserializeIncomingDaq(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize)
{
	uint8_t TimestampSize = 4; //TODO: get timestamp size from master (GetDaqResoultionInfo)
	return new DTO(Data, HeaderSize, TailSize, TimestampSize, false, m_Master.GetSlaveProperties().DaqProperies.IdentificationFieldType, m_Master.GetDaqLayout());
}

PacketFactory::PacketFactory(XCPMaster& master) : m_Master(master)
{
}


PacketFactory::~PacketFactory()
{
}

IXCPPacket * PacketFactory::CreateConnectPacket(ConnectPacket::ConnectMode mode)
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

IXCPPacket * PacketFactory::CreateAllocDaqPacket(uint16_t DaqCount, bool LittleEndian)
{
	AllocDaqPacket* packet = new AllocDaqPacket();
	packet->SetDaqCount(DaqCount, LittleEndian);
	return packet;
}

IXCPPacket * PacketFactory::CreateAllocOdtPacket(uint16_t DaqListNumber, uint8_t OdtCount, bool LittleEndian)
{
	AllocOdtPacket* packet = new AllocOdtPacket();
	packet->SetDaqListNumber(DaqListNumber, LittleEndian);
	packet->SetOdtCount(OdtCount);
	return packet;
}

IXCPPacket * PacketFactory::CreateAllocOdtEntryPacket(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntriesCount, bool LittleEndian)
{
	AllocOdtEntryPacket* packet = new AllocOdtEntryPacket();
	packet->SetDaqListNumber(DaqListNumber, LittleEndian);
	packet->SetOdtNumber(OdtNumber);
	packet->SetOdtEntriesCount(OdtEntriesCount);
	return packet;
}

IXCPPacket * PacketFactory::CreateSetDaqPtrPacket(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntryNumber, bool LittleEndian)
{
	SetDaqPtrPacket* packet = new SetDaqPtrPacket();
	packet->SetDaqListNumber(DaqListNumber, LittleEndian);
	packet->SetOdtNumber(OdtNumber);
	packet->SetOdtEntryNumber(OdtEntryNumber);
	return packet;
}

IXCPPacket * PacketFactory::CreateWriteDaqPacket(uint8_t BitOffset, uint8_t Size, uint8_t AddressExtension, uint32_t Address, bool LittleEndian)
{
	WriteDaqPacket* packet = new WriteDaqPacket();
	packet->SetBitOffset(BitOffset);
	packet->SetElementSize(Size);
	packet->SetAddressExtension(AddressExtension);
	packet->SetAddress(Address, LittleEndian);
	return packet;
}

IXCPPacket * PacketFactory::CreateSetDaqListModePacket(uint8_t Mode, uint16_t DaqListNumber, uint16_t EventChannel, uint8_t Prescaler, uint8_t Priority, bool LittleEndian)
{
	SetDaqListModePacket* packet = new SetDaqListModePacket(Mode, DaqListNumber, EventChannel, Prescaler, Priority, LittleEndian);
	return packet;
}

IXCPPacket * PacketFactory::CreateStartStopDaqListPacket(uint8_t Mode, uint16_t DaqListNumber, bool LittleEndian)
{
	return new StartStopDaqListPacket(Mode, DaqListNumber, LittleEndian);
}

IXCPPacket * PacketFactory::CreateStartStopSyncPacket(StartStopSynchPacket::Mode Mode)
{
	return new StartStopSynchPacket(Mode);
}

IXCPPacket * PacketFactory::CreateGetSeedPacket(GetSeedPacket::Mode Mode, GetSeedPacket::Resource Resource)
{
	return new GetSeedPacket(Mode,Resource);
}

IXCPPacket * PacketFactory::CreateGetDaqProcessorInfoPacket()
{
	return new GetDaqProcessorInfo();
}

IXCPPacket * PacketFactory::CreateClearDaqListPacket(uint16_t DaqListNumber, bool LittleEndian)
{
	return new ClearDAQListPacket(DaqListNumber, LittleEndian);
}

std::vector<IXCPPacket*> PacketFactory::CreateUnlockPackets(const std::vector<uint8_t>& Key)
{
	return UnlockPacket::CreateUnlockPackets(Key, m_Master.GetSlaveProperties().MaxCto);
}

IXCPPacket * PacketFactory::DeserializeIncomingFromSlave(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize, CommandPacket* LastSentCommand)
{
	if (Data.size() > HeaderSize) //Handle empty messages
	{
		uint8_t PID = Data[HeaderSize];
		switch (PID)
		{
		case CTOSlaveToMasterPacketTypes::RES:
			return CreateResponsePacket(Data, HeaderSize, TailSize, LastSentCommand);
			break;
		case CTOSlaveToMasterPacketTypes::EV:
			break;
		case CTOSlaveToMasterPacketTypes::ERR:
			return CreateErrorPacket(Data, HeaderSize, TailSize, LastSentCommand);
			break;
		case CTOSlaveToMasterPacketTypes::SERV:
			break;
		default:
			if (PID >= 0x00 && PID <= 0xFB)
			{
				return DeserializeIncomingDaq(Data, HeaderSize, TailSize);
			}
			break;
		}
	}
	return nullptr;
}
