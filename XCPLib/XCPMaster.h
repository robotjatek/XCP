#pragma once
#include "IMessageFactory.h"
#include "IXCPMessage.h"
#include "ConnectPacket.h"
#include "DisconnectPacket.h"
#include "PacketFactory.h"
#include "TCPMessage.h"
#include "IIncomingMessageHandler.h"
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

class XCP_API XCPMaster
{
private:
	IMessageFactory* m_MessageFactory;
	PacketFactory* m_PacketFactory;
	std::queue<CommandPacket*> m_SentCommandQueue;
	IIncomingMessageHandler* m_MessageHandler;

	struct SlaveProperties
	{
		bool CAL_PG;
		bool DAQ;
		bool STIM;
		bool PGM;


	};
public:
	XCPMaster(TransportLayer transportlayer);
	virtual ~XCPMaster();
	std::unique_ptr<IXCPMessage> CreateConnectMessage(ConnectMode mode);
	std::unique_ptr<IXCPMessage> CreateDisconnectMessage();
	std::unique_ptr<IXCPMessage> DeserializeMessage(std::vector<uint8_t>& data);
	void SendMessage(IXCPMessage* Message);
	IXCPMessage* ReceiveMessage(IXCPMessage* Message);
	void AddSentMessage(IXCPMessage* Packet);
};

