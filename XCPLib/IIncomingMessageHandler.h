#pragma once

class ConnectPositivePacket;
class DisconnectPacket;
class ResponsePacket;
class GetStatusResponsePacket;
class SynchResponsePacket;
class UploadResponse;

class IIncomingMessageHandler
{
public:
	virtual void Handle(ConnectPositivePacket& Packet) = 0;
	virtual void Handle(ResponsePacket& Packet) = 0;
	virtual void Handle(GetStatusResponsePacket& Packet) = 0;
	virtual void Handle(SynchResponsePacket& Packet) = 0;
	virtual void Handle(UploadResponse& Packet) = 0;
};