#pragma once
#include <cstdint>
#include <vector>
#include "IIncomingMessageHandler.h"
#include <iostream>

#ifdef XCPLIB_EXPORTS //XCPLIB_EXPORTS macro is defined in the project settings
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif


class XCP_API IXCPPacket
{
protected:
	uint8_t m_PID;
	uint8_t* m_Data;
	uint32_t m_DataLength;
	uint32_t m_PacketSize;
public:
	IXCPPacket() {}
	virtual void Serialize(std::vector<uint8_t>& out_Binary);
	uint32_t GetDataLength();
	virtual uint32_t GetPacketSize();
	virtual ~IXCPPacket();
	uint8_t GetPid();
	virtual void Dispatch(IIncomingMessageHandler& Handler)
	{
		std::cout << "Not implemented dispatch for this packet type\n";
	}
};