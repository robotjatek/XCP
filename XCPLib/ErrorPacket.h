#include "ICTO.h"

enum ErrorCodes
{

};

class ErrorPacket : public ICTO
{
public:
	ErrorPacket() : ICTO()
	{
		m_PID = CTOSlaveToMasterPacketTypes::ERR;
		m_Data = nullptr;
	}

	virtual void SetErrorCode(uint8_t ErrorCode)
	{
		m_Data[0] = ErrorCode;
	}

	virtual uint8_t GetErrorCode()
	{
		return m_Data[0];
	}
};