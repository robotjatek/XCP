#include "IncomingMessageHandler.h"
#include <iostream>
#include <iomanip>
#include "IXCPMessage.h"
#include "DisconnectPacket.h"

IncomingMessageHandler::IncomingMessageHandler(XCPMaster& Master) : m_Master(Master)
{
	//m_Master.CreateDisconnectMessage();
}


IncomingMessageHandler::~IncomingMessageHandler()
{
}

void IncomingMessageHandler::Handle(ConnectPositivePacket & Packet)
{
	std::cout << std::hex << std::setw(2)
		<< "PID: " << (int)Packet.GetPid()
		<< " | Resource:" << (int)Packet.GetResource()
				<< " (CAL/PG: " << ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::CAL_PG) != 0) << ", "
				<< "DAQ: " << ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::DAQ) != 0) << ", "
				<< "STIM: " << ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::STIM) != 0) << ", "
				<< "PGM: " << ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::PGM) != 0) << ") "
		<< " | CommMode:" << (int)Packet.GetCommModeBasic()
		<< " | MaxCTO:" << (int)Packet.GetMaxCto()
		<< " | MaxDTO:" << (int)Packet.GetMaxDto((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::BYTE_ORDER) == 0)
		<< " | ProtocolLayer version:" << (int)Packet.GetProtocolLayerVersion()
		<< " | Transport Layer version:" << (int)Packet.GetTransportLayerVersion() << "\n";

	std::cout << "Byte order: " << ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::BYTE_ORDER) != 0)<<"\n";
	std::cout << "Address granularity: " << ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::ADDRESS_GRANULARITY_BOTH) >>1) << "\n";
	std::cout << "Slave block mode: " << ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::SLAVE_BLOCK_MODE) != 0) << "\n";
	std::cout << "Optional data: " << ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::OPTIONAL) != 0) << "\n";
}

void IncomingMessageHandler::Handle(ResponsePacket & Packet)
{
	std::cout << "General Ack packet received (or an unhandled packet format) PID: "<<(int)Packet.GetPid()<<"\n";
}
