#include "IncomingMessageHandler.h"
#include <iostream>
#include <iomanip>
#include "IXCPMessage.h"
#include "DisconnectPacket.h"
#include "GetStatusPacket.h"
#include "UploadPacket.h"
#include "ErrorAccessLockedPacket.h"
#include "ErrorOutOfRangePacket.h"
#include "ErrorSequencePacket.h"
#include "ErrorMemoryOverflowPacket.h"
#include "UnlockPacket.h"
#include "DTO.h"
#include "ErrorCommandUnknown.h"

void IncomingMessageHandler::ResetSeedAndKey()
{
	m_ProcessedSeedLength = 0;
	m_RemainingSeedLength = -1;
	m_Key.clear();
	m_Key.resize(255);
	m_KeyLength = 255;
	m_SeedBytes.clear();
}

IncomingMessageHandler::IncomingMessageHandler(XCPMaster& Master) : m_Master(Master)
{
	ResetSeedAndKey();
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
	std::cout << "General Ack packet received (or an unhandled packet format) PID: " << (int)Packet.GetPid() << "\n";
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
		std::cout << std::hex << (int)Packet.GetElement<uint8_t>(i, (m_Master.GetSlaveProperties().ByteOrder == 0)) << " ";
	}
	std::cout << "\n";
}

void IncomingMessageHandler::Handle(ErrorAccessLockedPacket & Packet)
{
	std::cout << "Error, Access Denied, Seed & Key is required\n";
}

void IncomingMessageHandler::Handle(ErrorOutOfRangePacket & Packet)
{
	std::cout << (int)Packet.GetErrorCode() << "\n";
	std::cout << "Error: OUT_OF_RANGE\n";
}

void IncomingMessageHandler::Handle(ErrorSequencePacket & Packet)
{
	std::cout << "Error: ERR_SEQUENCE\n";
}

void IncomingMessageHandler::Handle(ErrorMemoryOverflowPacket & Packet)
{
	std::cout << "Error: Memory overflow\n";
}

void IncomingMessageHandler::Handle(StartStopDaqListPositiveResponse & Packet)
{
	if (m_Master.GetDaqLayout().IsInitialized())
	{
		std::cout << "Start/Stop Daq list response | FIRST PID: " << std::hex << (int)Packet.GetFirstPid() << "\n";
		StartStopDaqListPacket* command = m_Master.GetLastSentPacket<StartStopDaqListPacket*>();
		if (command)
		{
			DAQLayout& dl = m_Master.GetDaqLayout();
			DAQ& d = dl.GetDAQ(command->GetDaqListNumber(m_Master.GetSlaveProperties().ByteOrder == 0));
			d.SetFirstPid(Packet.GetFirstPid());
		}
		else
		{
			std::cout << "There was an error while setting the first PID on the DAQListDescriptor\a\n";
		}
	}
	else
	{
		std::cout << "Warning! Daq layout was not initialized before starting daq transfer.\n";
	}
}

void IncomingMessageHandler::Handle(GetSeedResponsePacket & Packet)
{
	std::cout << "GetSeedResponse\n";
	if (m_SeedBytes.size() == 0) //Mode 0
	{
		m_RemainingSeedLength = Packet.GetLengthField();
	}

	unsigned int SeedPartSize = Packet.GetSeedPartSize();
	for (unsigned int i = 0; i < SeedPartSize; i++)
	{
		m_SeedBytes.push_back(Packet.GetSeedPart(i));
	}
	
	m_ProcessedSeedLength += SeedPartSize;
	m_RemainingSeedLength -= SeedPartSize;
	GetSeedPacket* LastPacket = m_Master.GetLastSentPacket<GetSeedPacket*>();
	if (m_RemainingSeedLength == 0)
	{
		XCP_ComputeKeyFromSeedPtr_t ptr = m_Master.GetComputeKeyPtr();
		if (ptr)
		{
			ptr(LastPacket->GetResource(), Packet.GetLengthField(), &m_SeedBytes[0], &m_KeyLength, &m_Key[0]);
			m_Key.resize(m_KeyLength);
		}
	}
}

void IncomingMessageHandler::Handle(UnlockResponsePacket & Packet)
{
	std::cout << "UnlockResponse packet\n";
	XCPMaster::SlaveProperties properties = m_Master.GetSlaveProperties();
	properties.CAL_PG = ((Packet.GetCurrentResourceProtection()&ConnectPositivePacket::ResourceParameterBits::CAL_PG) != 0);
	properties.DAQ = ((Packet.GetCurrentResourceProtection()&ConnectPositivePacket::ResourceParameterBits::DAQ) != 0);
	properties.PGM = ((Packet.GetCurrentResourceProtection()&ConnectPositivePacket::ResourceParameterBits::PGM) != 0);
	properties.STIM = ((Packet.GetCurrentResourceProtection()&ConnectPositivePacket::ResourceParameterBits::STIM) != 0);
	m_Master.SetSlaveProperties(properties);

	std::cout << "(CAL/PG: " << properties.CAL_PG << ", "
		<< "DAQ: " << properties.DAQ << ", "
		<< "STIM: " << properties.STIM << ", "
		<< "PGM: " << properties.PGM << ") \n";
	ResetSeedAndKey();
}

void IncomingMessageHandler::Handle(GetDaqProcessorInfoResponse & Packet)
{
	std::cout << "GetDaqProcessorInfoResponsePacket\n";
	XCPMaster::SlaveProperties properties = m_Master.GetSlaveProperties();
	properties.DaqProperies.ConfigType = ((Packet.GetDaqProperties()&GetDaqProcessorInfoResponse::DaqPropertiesBits::DAQ_CONFIG_TYPE) != 0);
	properties.DaqProperies.PrescalerSupported = ((Packet.GetDaqProperties()&GetDaqProcessorInfoResponse::DaqPropertiesBits::PRESCALER_SUPPORTED) != 0);
	properties.DaqProperies.ResumeSupported = ((Packet.GetDaqProperties()&GetDaqProcessorInfoResponse::DaqPropertiesBits::RESUME_SUPPORTED) != 0);
	properties.DaqProperies.BitStimSupported = ((Packet.GetDaqProperties()&GetDaqProcessorInfoResponse::DaqPropertiesBits::BIT_STIM_SUPPORTED) != 0);
	properties.DaqProperies.TimeStampSupported = ((Packet.GetDaqProperties()&GetDaqProcessorInfoResponse::DaqPropertiesBits::TIMESTAMP_SUPPORTED) != 0);
	properties.DaqProperies.PidOffSupported = ((Packet.GetDaqProperties()&GetDaqProcessorInfoResponse::DaqPropertiesBits::PID_OFF_SUPPORTED) != 0);

	properties.DaqProperies.OverloadIndicationMode = Packet.GetDaqProperties()&GetDaqProcessorInfoResponse::DaqPropertiesBits::OVERLOAD_INDICATION_MODE;
	properties.DaqProperies.OptimisationType = Packet.GetDaqKeyByte()&GetDaqProcessorInfoResponse::DaqKeyByteBits::OPTIMISATION_TYPE;
	properties.DaqProperies.AddressExtensionType = Packet.GetDaqKeyByte()&GetDaqProcessorInfoResponse::DaqKeyByteBits::ADDRESS_EXTENSION;
	properties.DaqProperies.IdentificationFieldType = Packet.GetDaqKeyByte()&GetDaqProcessorInfoResponse::DaqKeyByteBits::IDENTIFICATION_FIELD_TYPE;

	properties.DaqProperies.MaxDaq = Packet.GetMaxDaq(properties.ByteOrder);
	properties.DaqProperies.MinDaq = Packet.GetMinDaq();
	properties.DaqProperies.MaxEventChannel = Packet.GetMaxEventChannel(properties.ByteOrder);

	m_Master.SetSlaveProperties(properties);

	std::cout << "Config type: ";
	if (properties.DaqProperies.ConfigType == 0)
	{
		std::cout << "Static DAQ\n";
	}
	else
	{
		std::cout << "Dynamic DAQ\n";
	}
	std::cout << "Prescaler supported: " << properties.DaqProperies.PrescalerSupported << "\n";
	std::cout << "Resume supported: " << properties.DaqProperies.ResumeSupported << "\n";
	std::cout << "BitStim supported: " << properties.DaqProperies.BitStimSupported << "\n";
	std::cout << "Timestamp supported: " << properties.DaqProperies.TimeStampSupported << "\n";
	std::cout << "Pid off supported: " << properties.DaqProperies.PidOffSupported << "\n";
	std::cout << "Overload indication mode: ";
	if (properties.DaqProperies.OverloadIndicationMode == GetDaqProcessorInfoResponse::OverloadIndicationMode::EVENT_PACKET)
	{
		std::cout << "Event Packet";
	}
	else if (properties.DaqProperies.OverloadIndicationMode == GetDaqProcessorInfoResponse::OverloadIndicationMode::MSB_PID)
	{
		std::cout << "MSB PID";
	}
	else if (properties.DaqProperies.OverloadIndicationMode == GetDaqProcessorInfoResponse::OverloadIndicationMode::NO_INDICATION)
	{
		std::cout << "No indication";
	}
	std::cout << "\n";
	std::cout << "Optimisation type: ";
	if (properties.DaqProperies.OptimisationType == GetDaqProcessorInfoResponse::OptimisationMode::OM_DEFAULT)
	{
		std::cout << "Default";
	}
	else if (properties.DaqProperies.OptimisationType == GetDaqProcessorInfoResponse::OptimisationMode::OM_ODT_TYPE_16)
	{
		std::cout << "OM_ODT_TYPE_16";
	}
	else if (properties.DaqProperies.OptimisationType == GetDaqProcessorInfoResponse::OptimisationMode::OM_ODT_TYPE_32)
	{
		std::cout << "OM_ODT_TYPE_32";
	}
	else if (properties.DaqProperies.OptimisationType == GetDaqProcessorInfoResponse::OptimisationMode::OM_ODT_TYPE_64)
	{
		std::cout << "OM_ODT_TYPE_64";
	}
	else if (properties.DaqProperies.OptimisationType == GetDaqProcessorInfoResponse::OptimisationMode::OM_MAX_ENTRY_SIZE)
	{
		std::cout << "OM_MAX_ENTRY_SIZE";
	}
	else if (properties.DaqProperies.OptimisationType == GetDaqProcessorInfoResponse::OptimisationMode::OM_ODT_TYPE_ALIGNMENT)
	{
		std::cout << "OM_ODT_TYPE_ALIGNMENT";
	}
	else
	{
		std::cout << "Unknown mode: " <<std::hex <<(int)properties.DaqProperies.OptimisationType;
	}
	std::cout << "\n";

	std::cout << "Address Extension type: ";
	if (properties.DaqProperies.AddressExtensionType == GetDaqProcessorInfoResponse::AddressExtensionType::CAN_BE_DIFFERENT)
	{
		std::cout << "address extension can be different within one and the same ODT";
	}
	else if (properties.DaqProperies.AddressExtensionType == GetDaqProcessorInfoResponse::AddressExtensionType::SAME_FOR_ALL_ENTRIES_ODT)
	{
		std::cout << "address extension to be the same for all entries within one ODT";
	}
	else if (properties.DaqProperies.AddressExtensionType == GetDaqProcessorInfoResponse::AddressExtensionType::SAME_FOR_ALL_ENTRIES_DAQ)
	{
		std::cout << "address extension to be the same for all entries within one DAQ";
	}
	else
	{
		std::cout << "unknown: " << std::hex << (int)properties.DaqProperies.AddressExtensionType;
	}
	std::cout << "\n";
	std::cout << "Identification field type: ";
	if (properties.DaqProperies.IdentificationFieldType == GetDaqProcessorInfoResponse::IdentificationFieldType::ABSOLUTE_ODT_NUMBER)
	{
		std::cout << "Absolute ODT number";
	}
	else if (properties.DaqProperies.IdentificationFieldType == GetDaqProcessorInfoResponse::IdentificationFieldType::RELATIVE_ODT_ABSOLUTE_DAQ_BYTE)
	{
		std::cout << "Relative ODT number, absolute DAQ list number (byte)";
	}
	else if (properties.DaqProperies.IdentificationFieldType == GetDaqProcessorInfoResponse::IdentificationFieldType::RELATIVE_ODT_ABSOLUTE_DAQ_WORD)
	{
		std::cout << "Relative ODT number, absolute DAQ list number (word)";
	}
	else if (properties.DaqProperies.IdentificationFieldType == GetDaqProcessorInfoResponse::IdentificationFieldType::RELATIVE_ODT_ABSOLUTE_DAQ_WORD_ALIGNED)
	{
		std::cout << "Relative ODT number, absolute DAQ list number (word-aligned)";
	}
	std::cout << "\n";
	std::cout << "Max DAQ: " << (int)properties.DaqProperies.MaxDaq<<"\n";
	std::cout << "Max Event channel: " << (int)properties.DaqProperies.MaxEventChannel<<"\n";
	std::cout << "Min DAQ: " << (int)properties.DaqProperies.MinDaq << "\n";
}

void IncomingMessageHandler::Handle(DTO & Packet)
{
	std::cout << "DAQ packet:\n";
	std::cout << Packet.GetDAQField()<<"\n";
	if (Packet.GetIsTimestamped())
	{
		std::cout << Packet.GetTimestamp() << "\n";
		DAQLayout& dl = m_Master.GetDaqLayout();
		DAQ& d = dl.GetDAQ(Packet.GetDAQIndex());
		d.SetLastTimestamp(Packet.GetTimestamp());
		//dl.SetDAQ(Packet.GetDAQIndex(), d);
		//m_Master.SetDaqLayout(dl);
	}
	for (unsigned int i = 0; i < Packet.GetDataLength(); i++)
	{
		std::cout << std::hex << (int)Packet.GetByteElement(i) << " ";
	}
	std::cout << "\n";
}

void IncomingMessageHandler::Handle(ErrorCommandUnknown & Packet)
{
	std::cout << "ERROR: Unknown command\n";
}

const std::vector<uint8_t>& IncomingMessageHandler::GetUnlockKey() const
{
	return m_Key;
}

