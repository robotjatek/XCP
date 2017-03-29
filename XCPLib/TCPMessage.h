#pragma once
#include "IXCPMessage.h"

class TCPMessage final : public IXCPMessage
{
private:
	//XCPPacket from the parent class
	//There is no Tail for a TCP message
	//The header is 4 bytes long for a XCPonTCP
public:
	TCPMessage(IXCPPacket*  packet, uint16_t ctr);
	~TCPMessage();
};
