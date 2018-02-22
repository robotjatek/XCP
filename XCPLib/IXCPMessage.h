#pragma once
#include <cstdint>
#include <vector>
#include "IXCPPacket.h"
#include "IXCPHeader.h"
#include "IXCPTail.h"

#include "XCPApiDecl.h"

class IXCPMessage
{
protected:
	IXCPHeader* m_Header;
	IXCPPacket* m_Packet;
	IXCPTail* m_Tail;
public:
	IXCPMessage();
	virtual ~IXCPMessage();
	IXCPPacket* GetPacket();
	void SetPacket(IXCPPacket* packet);
	virtual void Serialize(std::vector<uint8_t>& out_Binary);
	uint8_t GetHeaderSize();
	uint8_t GetTailSize();
};
