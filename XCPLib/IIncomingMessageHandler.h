#pragma once

class ConnectPositivePacket;
class DisconnectPacket;
class ResponsePacket;

class IIncomingMessageHandler
{
public:
	virtual void Handle(ConnectPositivePacket& Packet) = 0;
	virtual void Handle(ResponsePacket& Packet) = 0;
};