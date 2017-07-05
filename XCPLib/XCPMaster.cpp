#include "XCPMaster.h"
#include "TCPMessageFactory.h"
#include "CANMessageFactory.h"
#include "IncomingMessageHandler.h"
//#include <vld.h>

const XCPMaster::SlaveProperties& XCPMaster::GetSlaveProperties() const
{
	return m_SlaveProperties;
}

void XCPMaster::SetSlaveProperties(const SlaveProperties& properties)
{
	m_SlaveProperties = properties;
}

XCPMaster::XCPMaster(TransportLayer transportlayer)
{
	switch (transportlayer)
	{
	case TransportLayer::ETHERNET:
		m_MessageFactory = new TCPMessageFactory();
		break;
	case TransportLayer::CAN:
		m_MessageFactory = new CANMessageFactory();
		break;
	default:
		m_MessageFactory = nullptr;
		break;
	}

	m_PacketFactory = new PacketFactory(*this);
	m_MessageHandler = new IncomingMessageHandler(*this);
	m_ExternalHandler = nullptr;
}

XCPMaster::~XCPMaster()
{
	delete m_MessageFactory;
	delete m_PacketFactory;
	delete m_MessageHandler;
}

std::unique_ptr<IXCPMessage> XCPMaster::CreateConnectMessage(ConnectPacket::ConnectMode mode)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateConnectPacket(mode)));
}

std::unique_ptr<IXCPMessage> XCPMaster::CreateDisconnectMessage()
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateDisconnectPacket()));
}

std::unique_ptr<IXCPMessage> XCPMaster::CreateGetStatusMessage()
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateGetStatusPacket()));
}

std::unique_ptr<IXCPMessage> XCPMaster::CreateSynchMessage()
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateSynchPacket()));
}

std::unique_ptr<IXCPMessage> XCPMaster::CreateSetMTAMessage(uint32_t address, uint8_t extension)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateSetMTAPacket(address, extension, m_SlaveProperties.ByteOrder == 0)));
}

std::unique_ptr<IXCPMessage> XCPMaster::DeserializeMessage(std::vector<uint8_t>& data)
{
	CommandPacket* LastCommand = nullptr;
	if (m_SentCommandQueue.size() > 0)
	{
		LastCommand = m_SentCommandQueue.front();
	}

	IXCPPacket* Packet = m_PacketFactory->DeserializeIncomingFromSlave(data, m_MessageFactory->GetHeaderSize(), m_MessageFactory->GetTailSize(), LastCommand);

	if (Packet)
	{
		std::vector<uint8_t>(data.begin() + m_MessageFactory->GetHeaderSize() + Packet->GetPacketSize() + m_MessageFactory->GetTailSize(), data.end()).swap(data); //Clear packet data from the buffer. Leaves the not-yet-deserialized data intact.
		IXCPMessage* MessageFrame = m_MessageFactory->CreateMessage(Packet);
		//Pass decoded packets to the internal and external handlers.
		if (m_MessageHandler)
		{
			Packet->Dispatch(*m_MessageHandler);
		}
		if (m_ExternalHandler)
		{
			Packet->Dispatch(*m_ExternalHandler);
		}
		std::cout << "---------------------------------------------\n";
		if (m_SentCommandQueue.size() > 0 && Packet->GetPid() > 0xFB) //do not pop if this was a DAQ packet
		{
			m_SentCommandQueue.pop();
		}
		return std::unique_ptr<IXCPMessage>(MessageFrame);
	}
	std::cout << "couldnt deserialise the message\n";
	std::cout << "---------------------------------------------\n";
	if (m_SentCommandQueue.size() > 0) //TODO: delete this after all packet types are handled correctly...
	{
		m_SentCommandQueue.pop();
	}
	data.clear();
	return nullptr;
}

std::unique_ptr<IXCPMessage> XCPMaster::CreateUploadMessage(uint8_t NumberOfElements)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateUploadPacket(NumberOfElements)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateShortUploadMessage(uint8_t NumberOfElements, uint32_t Address, uint8_t AddressExtension)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateShortUploadPacket(NumberOfElements, Address, AddressExtension, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateFreeDaqMessage()
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateFreeDaqPacket()));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateAllocDaqMessage(uint16_t DaqCount)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateAllocDaqPacket(DaqCount, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateAllocOdtMessage(uint16_t DaqListNumber, uint8_t OdtCount)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateAllocOdtPacket(DaqListNumber, OdtCount, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateAllocOdtEntryMessage(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntryCount)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateAllocOdtEntryPacket(DaqListNumber, OdtNumber, OdtEntryCount, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateSetDaqPtrMessage(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntryNumber)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateSetDaqPtrPacket(DaqListNumber, OdtNumber, OdtEntryNumber, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateWriteDaqMessage(uint8_t BitOffset, uint8_t ElementSize, uint8_t AddressExtension, uint32_t Address)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateWriteDaqPacket(BitOffset, ElementSize, AddressExtension, Address, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateSetDaqListModeMessage(uint8_t Mode, uint16_t DaqListNumber, uint16_t EventChannel, uint8_t Prescaler, uint8_t Priority)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateSetDaqListModePacket(Mode, DaqListNumber, EventChannel, Prescaler, Priority, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateStartStopDaqListMessage(StartStopDaqListPacket::Mode Mode, uint16_t DaqListNumber)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}

	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateStartStopDaqListPacket(Mode, DaqListNumber, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateStartStopSynchMessage(StartStopSynchPacket::Mode Mode)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}
	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateStartStopSyncPacket(Mode)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateGetSeedMessage(GetSeedPacket::Mode Mode, GetSeedPacket::Resource Resource)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}
	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateGetSeedPacket(Mode, Resource)));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateGetDaqProcessorInfoMessage()
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}
	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateGetDaqProcessorInfoPacket()));
}

XCP_API std::unique_ptr<IXCPMessage> XCPMaster::CreateClearDaqListMessage(uint16_t DaqListNumber)
{
	if (!m_MessageFactory)
	{
		return nullptr;
	}
	return std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(m_PacketFactory->CreateClearDaqListPacket(DaqListNumber, m_SlaveProperties.ByteOrder == 0)));
}

XCP_API std::vector<std::unique_ptr<IXCPMessage>> XCPMaster::CreateUnlockMessages()
{
	std::vector<std::unique_ptr<IXCPMessage>> ret;
	const std::vector<IXCPPacket*>& packets = m_PacketFactory->CreateUnlockPackets(m_MessageHandler->GetUnlockKey());
	for (const auto& a : packets)
	{
		ret.push_back(std::move(std::unique_ptr<IXCPMessage>(m_MessageFactory->CreateMessage(a))));
	}

	return ret;
}

void XCPMaster::AddSentMessage(IXCPMessage * Packet)
{
	if (CommandPacket* ToAdd = dynamic_cast<CommandPacket*>(Packet->GetPacket()))
	{
		m_SentCommandQueue.push(ToAdd);
	}
	else
	{
		std::cout << "XCPMaster::AddSentMessage: This is not a CMD packet.";
	}
}

XCP_API void XCPMaster::SetSeedAndKeyFunctionPointers(XCP_GetAvailablePrivilegesPtr_t GetAvailablePrivilegesPtr, XCP_ComputeKeyFromSeedPtr_t ComputeKeyPtr)
{
	m_GetAvailablePrivileges = GetAvailablePrivilegesPtr;
	m_ComputeKeyFromSeed = ComputeKeyPtr;
}

XCP_API DAQLayout& XCPMaster::GetDaqLayout()
{
	return m_DAQLayout;
}

XCP_API void XCPMaster::SetDaqLayout(DAQLayout layout)
{
	m_DAQLayout = layout;
}

XCP_API void XCPMaster::SetExternalMessageHandler(IIncomingMessageHandler * Handler)
{
	m_ExternalHandler = Handler;
}

XCP_ComputeKeyFromSeedPtr_t XCPMaster::GetComputeKeyPtr()
{
	return m_ComputeKeyFromSeed;
}

XCP_GetAvailablePrivilegesPtr_t XCPMaster::GetAvailablePrivilegesPtr()
{
	return m_GetAvailablePrivileges;
}
