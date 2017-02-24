#pragma once
#include "IXCPMessage.h"

class CANMessage : public IXCPMessage
{
public:
	virtual uint8_t* GetRawBytes();
	virtual ~CANMessage() {};
};