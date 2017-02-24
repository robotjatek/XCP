#pragma once
#include "IXCPPacket.h"
#include "ConnectPacket.h"
#include "DisconnectPacket.h"
#include "ICTO.h"

class PacketFactory
{
private:
	IXCPPacket* CreateResponsePacket(std::vector<uint8_t>& data, uint8_t header_size, CommandPacket* LastSentCommand);
public:
	PacketFactory();
	virtual ~PacketFactory();
	IXCPPacket* CreateConnectPacket(ConnectMode Mode);
	IXCPPacket* CreateDisconnectPacket();
	IXCPPacket* DeserializeIncomingFromSlave(std::vector<uint8_t>& Data, uint8_t HeaderSize, CommandPacket* LastSentCommand);
};

