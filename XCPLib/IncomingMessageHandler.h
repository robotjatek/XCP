#pragma once
#include "IIncomingMessageHandler.h"
#include "ConnectPositivePacket.h"
#include "XCPMaster.h"

class IncomingMessageHandler :
	public IIncomingMessageHandler
{
private:
	XCPMaster& m_Master;
public:
	IncomingMessageHandler(XCPMaster& Master);
	virtual ~IncomingMessageHandler();
	virtual void Handle(ConnectPositivePacket& Packet);
	virtual void Handle(ResponsePacket& Packet);
	virtual void Handle(GetStatusResponsePacket& Packet);
	virtual void Handle(SynchResponsePacket& Packet);
	virtual void Handle(UploadResponse& Packet);
};

