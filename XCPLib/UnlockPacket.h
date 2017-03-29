#pragma once
#include "CommandPacket.h"
#include "ResponsePacket.h"

class UnlockPacket final :
	public CommandPacket
{
private:
	enum BytePositions
	{
		REMAINING_KEY_LENGTH = 0x0,
		KEY = 0x1, //1..maxcto-2
	};
public:
	UnlockPacket(const std::vector<uint8_t>& Key, uint8_t StartIndex, uint8_t MaxCto);
	virtual ~UnlockPacket();
	static std::vector<IXCPPacket*> CreateUnlockPackets(const std::vector<uint8_t>& Key, uint8_t MaxCto);
	uint8_t GetRemainingKeyLength();
};

class UnlockResponsePacket final : public ResponsePacket
{
	enum BytePositions
	{
		CURRENT_RESOURCE_PROTECTION_STATUS = 0x0,
	};
public:
	UnlockResponsePacket(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize);
	virtual ~UnlockResponsePacket();
	void SetCurrentResourceProtection(uint8_t CurrentProection);
	uint8_t GetCurrentResourceProtection();
	virtual void Dispatch(IIncomingMessageHandler& Handler);
	static UnlockResponsePacket* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize);
};

