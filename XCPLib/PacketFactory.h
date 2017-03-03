#pragma once
#include "IXCPPacket.h"
#include "CommandPacket.h"
#include "ICTO.h"
#include "ConnectPacket.h"

class XCPMaster;

class PacketFactory
{
private:
	IXCPPacket* CreateResponsePacket(const std::vector<uint8_t>& data, uint8_t header_size, CommandPacket* LastSentCommand);
	IXCPPacket* CreateErrorPacket(const std::vector<uint8_t>& data, uint8_t header_size, CommandPacket* LastSentCommand);
	XCPMaster& m_Master;
public:
	PacketFactory(XCPMaster& master);
	virtual ~PacketFactory();
	IXCPPacket* CreateConnectPacket(ConnectMode Mode);
	IXCPPacket* CreateDisconnectPacket();
	IXCPPacket* CreateGetStatusPacket();
	IXCPPacket* CreateSynchPacket();
	IXCPPacket* CreateSetMTAPacket(uint32_t address, uint8_t extension, bool LittleEndian);
	IXCPPacket* CreateUploadPacket(uint8_t NumberOfElements);
	IXCPPacket* CreateShortUploadPacket(uint8_t NumberOfElements, uint32_t Address, uint8_t AddressExtension, bool LittleEndian);
	IXCPPacket* CreateFreeDaqPacket();
	IXCPPacket* DeserializeIncomingFromSlave(const std::vector<uint8_t>& Data, uint8_t HeaderSize, CommandPacket* LastSentCommand);
};

