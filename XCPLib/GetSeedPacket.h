#pragma once
#include "CommandPacket.h"
#include "ResponsePacket.h"
class GetSeedPacket final :
	public CommandPacket
{
private:
	enum BytePositions
	{
		MODE = 0x0,
		RESOURCE = 0x1,
	};
public:
	enum Mode
	{
		FIRST_PART = 0x00,
		REMAINING_PART = 0x01
	};

	enum Resource
	{
		CAL_PG = 0x01,
		DAQ = 0x04,
		STIM = 0x08,
		PGM = 0x10,
	};
	GetSeedPacket();
	GetSeedPacket(Mode Mode, Resource Resource);
	virtual ~GetSeedPacket();
	void SetMode(Mode Mode);
	void SetResource(Resource Resource);
	uint8_t GetResource();
};

//--------------------------------------------------------

class GetSeedResponsePacket final : public ResponsePacket
{
	enum BytePositions
	{
		LENGTH = 0x00,
		SEED = 0x01,
	};
public:
	GetSeedResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize);
	virtual ~GetSeedResponsePacket();
	uint8_t GetLengthField();
	uint8_t GetSeedPart(uint8_t Element);
	uint8_t GetSeedPartSize();
	static GetSeedResponsePacket* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize);
	virtual void Dispatch(IIncomingMessageHandler& Handler);
};
