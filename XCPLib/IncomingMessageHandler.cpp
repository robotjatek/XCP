#include "IncomingMessageHandler.h"
#include <iostream>
#include <iomanip>
#include "IXCPMessage.h"
#include "DisconnectPacket.h"
#include "GetStatusPacket.h"
#include "UploadPacket.h"
#include "ErrorAccessLockedPacket.h"
#include "ErrorOutOfRangePacket.h"

IncomingMessageHandler::IncomingMessageHandler(XCPMaster& Master) : m_Master(Master)
{
}


IncomingMessageHandler::~IncomingMessageHandler()
{
}

void IncomingMessageHandler::Handle(ConnectPositivePacket & Packet)
{
	XCPMaster::SlaveProperties properties = m_Master.GetSlaveProperties();

	properties.CAL_PG = ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::CAL_PG) != 0);
	properties.DAQ = ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::DAQ) != 0);
	properties.PGM = ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::PGM) != 0);
	properties.STIM = ((Packet.GetResource()&ConnectPositivePacket::ResourceParameterBits::STIM) != 0);

	properties.ByteOrder = ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::BYTE_ORDER) != 0);
	properties.AddressGranularity = ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::ADDRESS_GRANULARITY_BOTH) >> 1);
	properties.SlaveBlockMode = ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::SLAVE_BLOCK_MODE) != 0);
	properties.OptionalData = ((Packet.GetCommModeBasic()&ConnectPositivePacket::CommModeBasicBits::OPTIONAL) != 0);

	properties.MaxCto = Packet.GetMaxCto();
	properties.MaxDto = Packet.GetMaxDto(properties.ByteOrder); //at this point we have already set the ByteOrder
	properties.TransportLayerVersion = Packet.GetTransportLayerVersion();
	properties.ProtocolLayerVersion = Packet.GetProtocolLayerVersion();
	
	m_Master.SetSlaveProperties(properties); //write back Slave properties to the Master

	std::cout << std::hex << std::setw(2)
		<< "PID: " << (int)Packet.GetPid()
		<< " | Resource:" << (int)Packet.GetResource()
		<< " (CAL/PG: " << properties.CAL_PG << ", "
		<< "DAQ: " << properties.DAQ << ", "
		<< "STIM: " << properties.STIM << ", "
		<< "PGM: " << properties.PGM << ") "
		<< " | CommMode:" << (int)Packet.GetCommModeBasic()
		<< " | MaxCTO:" << (int)properties.MaxCto
		<< " | MaxDTO:" << (int)properties.MaxDto
		<< " | ProtocolLayer version:" << (int)properties.ProtocolLayerVersion
		<< " | Transport Layer version:" << (int)properties.TransportLayerVersion << "\n";

	std::cout << "Byte order: " << properties.ByteOrder << "\n";
	std::cout << "Address granularity: " << (int)properties.AddressGranularity << "\n";
	std::cout << "Slave block mode: " << properties.SlaveBlockMode << "\n";
	std::cout << "Optional data: " << properties.OptionalData << "\n";
}

void IncomingMessageHandler::Handle(ResponsePacket & Packet)
{
	std::cout << "General Ack packet received (or an unhandled packet format) PID: "<<(int)Packet.GetPid()<<"\n";
}

void IncomingMessageHandler::Handle(GetStatusResponsePacket & Packet)
{
	std::cout << "jott egy statusresonse packet\n";
}

void IncomingMessageHandler::Handle(SynchResponsePacket & Packet)
{
	std::cout << "jott egy sync response...\n";
}

void IncomingMessageHandler::Handle(UploadResponse & Packet)
{
	std::cout << "Jott egy Upload response\nTartalma:\n";
	for (uint8_t i = 0; i < Packet.GetNumberOfElements(); i++)
	{
		std::cout << std::hex<<(int)Packet.GetElement<uint8_t>(i,(m_Master.GetSlaveProperties().ByteOrder == 0)) << " ";
	}
	std::cout << "\n";
}

void IncomingMessageHandler::Handle(ErrorAccessLockedPacket & Packet)
{
	std::cout << "Error, Access Denied, Seed & Key is required\n";
}

void IncomingMessageHandler::Handle(ErrorOutOfRangePacket & Packet)
{
	std::cout << "DAQ error: OUT_OF_RANGE\n";
}



