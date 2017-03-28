#pragma once
#include "IIncomingMessageHandler.h"
#include <DTO.h>
class XCPWorkerThread;

class IncomingHandlerExternal :
	public IIncomingMessageHandler
{
	XCPWorkerThread* m_wnd;
public:
	IncomingHandlerExternal(XCPWorkerThread* wnd);
	virtual ~IncomingHandlerExternal();
	virtual void Handle(ConnectPositivePacket& Packet) {};
	virtual void Handle(ResponsePacket& Packet) {};
	virtual void Handle(GetStatusResponsePacket& Packet) {};
	virtual void Handle(SynchResponsePacket& Packet) {};
	virtual void Handle(UploadResponse& Packet) {};
	virtual void Handle(ErrorAccessLockedPacket& Packet) {};
	virtual void Handle(ErrorOutOfRangePacket& Packet) {};
	virtual void Handle(ErrorSequencePacket& Packet) {};
	virtual void Handle(ErrorMemoryOverflowPacket& Packet) {};
	virtual void Handle(StartStopDaqListPositiveResponse& Packet) {};
	virtual void Handle(GetSeedResponsePacket& Packet) {};
	virtual void Handle(UnlockResponsePacket& Packet) {};
	virtual void Handle(GetDaqProcessorInfoResponse& Packet) {};
	virtual void Handle(DTO& Packet);
	std::vector<uint8_t> asd;
	virtual const std::vector<uint8_t>& GetUnlockKey() const { return asd; };
};

