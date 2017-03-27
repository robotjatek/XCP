#pragma once
#include "IXCPPacket.h"
#include "SlaveMemory.h"

#ifdef XCPLIB_EXPORTS //XCPLIB_EXPORTS macro is defined in the project settings
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif

class DTO :
	public IXCPPacket
{
private:
	uint16_t m_DAQ;
	uint8_t m_Fill; //If in aligned DAQ mode, this is the ctr field
	uint8_t m_CTR;
	uint32_t m_Timestamp;
	bool IsTimestamped;
	bool IsCTRed;
public:
	DTO(const std::vector<uint8_t>& Data, uint8_t HeadSize, uint8_t TailSize, uint8_t TimestampSize, bool TimestampFixed, uint8_t IdentificationFieldType, DAQLayout& DaqLayout);
	virtual ~DTO();
	XCP_API uint16_t GetDAQField();
	XCP_API uint8_t GetCTRField();
	XCP_API uint8_t GetFillField();
	XCP_API uint32_t GetTimestamp();
	XCP_API uint8_t GetByteElement(uint32_t i);
	XCP_API bool GetIsTimestamped();
	XCP_API bool GetIsCTRed();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

