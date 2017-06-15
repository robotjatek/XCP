#pragma once

class ConnectPositivePacket;
class DisconnectPacket;
class ResponsePacket;
class GetStatusResponsePacket;
class SynchResponsePacket;
class UploadResponse;
class ErrorAccessLockedPacket;
class ErrorOutOfRangePacket;
class ErrorSequencePacket;
class ErrorMemoryOverflowPacket;
class StartStopDaqListPositiveResponse;
class GetSeedResponsePacket;
class UnlockResponsePacket;
class GetDaqProcessorInfoResponse;
class DTO;
class ErrorCommandUnknown;
#include <vector>

class IIncomingMessageHandler
{
public:
	virtual void Handle(ConnectPositivePacket& Packet) = 0;
	virtual void Handle(ResponsePacket& Packet) = 0;
	virtual void Handle(GetStatusResponsePacket& Packet) = 0;
	virtual void Handle(SynchResponsePacket& Packet) = 0;
	virtual void Handle(UploadResponse& Packet) = 0;
	virtual void Handle(ErrorAccessLockedPacket& Packet) = 0;
	virtual void Handle(ErrorOutOfRangePacket& Packet) = 0;
	virtual void Handle(ErrorSequencePacket& Packet) = 0;
	virtual void Handle(ErrorMemoryOverflowPacket& Packet) = 0;
	virtual void Handle(StartStopDaqListPositiveResponse& Packet) = 0;
	virtual void Handle(GetSeedResponsePacket& Packet) = 0;
	virtual void Handle(UnlockResponsePacket& Packet) = 0;
	virtual void Handle(GetDaqProcessorInfoResponse& Packet) = 0;
	virtual void Handle(DTO& Packet) = 0;
	virtual void Handle(ErrorCommandUnknown& Packet) = 0;
	virtual const std::vector<uint8_t>& GetUnlockKey() const = 0;
	virtual ~IIncomingMessageHandler() {};
};