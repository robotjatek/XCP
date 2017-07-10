#pragma once
#include "IMessageFactory.h"
#include "IXCPMessage.h"
#include "ConnectPacket.h"
#include "DisconnectPacket.h"
#include "PacketFactory.h"
#include "TCPMessage.h"
#include "DAQPackets.h"
#include "IIncomingMessageHandler.h"
#include "SlaveMemory.h"
#include <queue>
#include <memory>

#ifdef XCPLIB_EXPORTS //XCPLIB_EXPORTS macro is defined in the project settings
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif

enum TransportLayer
{
	CAN,
	ETHERNET,
	SXL,
	USB,
	FLEXRAY
};


typedef uint32_t(*XCP_GetAvailablePrivilegesPtr_t)(uint8_t* AvailablePrivilege);
typedef uint32_t(*XCP_ComputeKeyFromSeedPtr_t)(uint8_t RequestedPrivilege, uint8_t ByteLenghtSeed, uint8_t* PointerToSeed, uint8_t* ByteLengthKey, uint8_t* PointerToKey);
using XCPMsgPtr = std::unique_ptr<IXCPMessage>;

class XCPMaster
{
public:
	struct SlaveProperties
	{
		bool CAL_PG;
		bool DAQ;
		bool STIM;
		bool PGM;

		bool ByteOrder;
		uint8_t AddressGranularity;
		bool SlaveBlockMode;
		bool OptionalData;

		uint16_t MaxDto;
		uint8_t MaxCto;
		uint8_t ProtocolLayerVersion;
		uint8_t TransportLayerVersion;

		struct
		{
			bool ConfigType;
			bool PrescalerSupported;
			bool ResumeSupported;
			bool BitStimSupported;
			bool TimeStampSupported;
			bool PidOffSupported;
			uint8_t OverloadIndicationMode;
			uint8_t OptimisationType;
			uint8_t AddressExtensionType;
			uint8_t IdentificationFieldType;
			uint16_t MaxDaq;
			uint16_t MaxEventChannel;
			uint8_t MinDaq;
			
		}DaqProperies;
	};

private:
	IMessageFactory* m_MessageFactory;
	PacketFactory* m_PacketFactory;
	std::queue<CommandPacket*> m_SentCommandQueue;
	IIncomingMessageHandler* m_MessageHandler;
	IIncomingMessageHandler* m_ExternalHandler;
	SlaveProperties m_SlaveProperties;	
	DAQLayout m_DAQLayout;

	XCP_GetAvailablePrivilegesPtr_t m_GetAvailablePrivileges = nullptr;
	XCP_ComputeKeyFromSeedPtr_t m_ComputeKeyFromSeed = nullptr;
	XCP_API XCPMaster();

public:
	XCP_API const SlaveProperties& GetSlaveProperties() const;
	XCP_API void SetSlaveProperties(const SlaveProperties& properties);
	XCP_API XCPMaster(TransportLayer transportlayer);
	XCP_API virtual ~XCPMaster();
	XCP_API std::unique_ptr<IXCPMessage> CreateConnectMessage(ConnectPacket::ConnectMode mode);
	XCP_API std::unique_ptr<IXCPMessage> CreateDisconnectMessage();
	XCP_API std::unique_ptr<IXCPMessage> CreateGetStatusMessage();
	XCP_API std::unique_ptr<IXCPMessage> CreateSynchMessage();
	XCP_API std::unique_ptr<IXCPMessage> CreateSetMTAMessage(uint32_t address, uint8_t extension);
	XCP_API std::unique_ptr<IXCPMessage> DeserializeMessage(std::vector<uint8_t>& data);
	XCP_API std::unique_ptr<IXCPMessage> CreateUploadMessage(uint8_t NumberOfElements);
	XCP_API std::unique_ptr<IXCPMessage> CreateShortUploadMessage(uint8_t NumberOfElements, uint32_t Address, uint8_t AddressExtension);
	XCP_API std::unique_ptr<IXCPMessage> CreateFreeDaqMessage();
	XCP_API std::unique_ptr<IXCPMessage> CreateAllocDaqMessage(uint16_t DaqCount);
	XCP_API std::unique_ptr<IXCPMessage> CreateAllocOdtMessage(uint16_t DaqListNumber, uint8_t OdtCount);
	XCP_API std::unique_ptr<IXCPMessage> CreateAllocOdtEntryMessage(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntryCount);
	XCP_API std::unique_ptr<IXCPMessage> CreateSetDaqPtrMessage(uint16_t DaqListNumber, uint8_t OdtNumber, uint8_t OdtEntryNumber);
	XCP_API std::unique_ptr<IXCPMessage> CreateWriteDaqMessage(uint8_t BitOffset, uint8_t ElementSize, uint8_t AddressExtension, uint32_t Address);
	XCP_API std::unique_ptr<IXCPMessage> CreateSetDaqListModeMessage(uint8_t Mode, uint16_t DaqListNumber, uint16_t EventChannel, uint8_t Prescaler, uint8_t Priority);
	XCP_API std::unique_ptr<IXCPMessage> CreateStartStopDaqListMessage(StartStopDaqListPacket::Mode Mode, uint16_t DaqListNumber);
	XCP_API std::unique_ptr<IXCPMessage> CreateStartStopSynchMessage(StartStopSynchPacket::Mode Mode);
	XCP_API std::unique_ptr<IXCPMessage> CreateGetSeedMessage(GetSeedPacket::Mode Mode, GetSeedPacket::Resource Resource);
	XCP_API std::unique_ptr<IXCPMessage> CreateGetDaqProcessorInfoMessage();
	XCP_API std::unique_ptr<IXCPMessage> CreateClearDaqListMessage(uint16_t DaqListNumber);
	XCP_API std::vector<std::unique_ptr<IXCPMessage>> CreateUnlockMessages();
	XCP_API void SendMessage(IXCPMessage* Message);
	XCP_API IXCPMessage* ReceiveMessage(IXCPMessage* Message);
	XCP_API void AddSentMessage(IXCPMessage* Packet);
	XCP_API void SetSeedAndKeyFunctionPointers(XCP_GetAvailablePrivilegesPtr_t GetAvailablePrivilegesPtr, XCP_ComputeKeyFromSeedPtr_t ComputeKeyPtr);
	XCP_API DAQLayout& GetDaqLayout();
	XCP_API void SetDaqLayout(DAQLayout layout);
	XCP_API void SetExternalMessageHandler(IIncomingMessageHandler* Handler);
	XCP_ComputeKeyFromSeedPtr_t GetComputeKeyPtr();
	XCP_GetAvailablePrivilegesPtr_t GetAvailablePrivilegesPtr();
	template<class PacketType>
	PacketType GetLastSentPacket();
};

template<class PacketType>
inline PacketType XCPMaster::GetLastSentPacket()
{
	return dynamic_cast<PacketType>(m_SentCommandQueue.front());
}
