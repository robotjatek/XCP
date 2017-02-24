#pragma once
#include <cstdint>
#include <vector>
#include "IXCPPacket.h"
#include "IXCPHeader.h"
#include "IXCPTail.h"

/*
#ifdef XCPLIB_EXPORTS //XCPLIB_EXPORTS macro is defined in the project settings
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif
*/

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