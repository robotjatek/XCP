#pragma once
#include "IXCPPacket.h"
#include "CommandPacket.h"
#include "ICTO.h"
#include "ConnectPacket.h"
#include "DAQPackets.h"
#include "GetSeedPacket.h"
#include "SlaveMemory.h"

class XCPMaster;

class PacketFactory
{
private:
	IXCPPacket* CreateResponsePacket(const std::vector<uint8_t>& data, uint8_t header_size, uint8_t TailSize, CommandPacket* LastSentCommand);
	IXCPPacket* CreateErrorPacket(const std::vector<uint8_t>& data, uint8_t header_size, uint8_t TailSize, CommandPacket* LastSentCommand);
	IXCPPacket* DeserializeIncomingDaq(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize);
	XCPMaster& m_Master;
public:
	PacketFactory(XCPMaster& master);
	virtual ~PacketFactory();
	IXCPPacket* CreateConnectPacket(ConnectPacket::ConnectMode Mode);
	IXCPPacket* CreateDisconnectPacket();
	IXCPPacket* CreateGetStatusPacket();
	IXCPPacket* CreateSynchPacket();
	IXCPPacket* CreateSetMTAPacket(uint32_t address, uint8_t extension, bool LittleEndian);
	IXCPPacket* CreateUploadPacket(uint8_t NumberOfElements);
	IXCPPacket* CreateShortUploadPacket(uint8_t NumberOfElements, uint32_t Address, uint8_t AddressExtension, bool LittleEndian);
	IXCPPacket* CreateFreeDaqPacket();
	IXCPPacket* CreateAllocDaqPacket(uint16_t DaqCount, bool LittleEndian);
	IXCPPacket* CreateAllocOdtPacket(uint16_t DaqListNumber, uint8_t OdtCount, bool LittleEndian);
	IXCPPacket* CreateAllocOdtEntryPacket(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntriesCount, bool LittleEndian);
	IXCPPacket* CreateSetDaqPtrPacket(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntryNumber, bool LittleEndian);
	IXCPPacket* CreateWriteDaqPacket(uint8_t BitOffset, uint8_t Size, uint8_t AddressExtension, uint32_t Address, bool LittleEndian);
	IXCPPacket* CreateSetDaqListModePacket(uint8_t Mode, uint16_t DaqListNumber, uint16_t EventChannel, uint8_t Prescaler, uint8_t Priority, bool LittleEndian);
	IXCPPacket* CreateStartStopDaqListPacket(uint8_t Mode, uint16_t DaqListNumber, bool LittleEndian);
	IXCPPacket* CreateStartStopSyncPacket(StartStopSynchPacket::Mode Mode);
	IXCPPacket* CreateGetSeedPacket(GetSeedPacket::Mode Mode, GetSeedPacket::Resource Resource);
	IXCPPacket* CreateGetDaqProcessorInfoPacket();
	IXCPPacket* CreateClearDaqListPacket(uint16_t DaqListNumber, bool LittleEndian);
	std::vector<IXCPPacket*> CreateUnlockPackets(const std::vector<uint8_t>& Key);
	IXCPPacket* DeserializeIncomingFromSlave(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize, CommandPacket* LastSentCommand);
};

