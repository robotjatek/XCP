#pragma once
#include "CommandPacket.h"
class SetMTAPacket final :
	public CommandPacket
{
	enum BytePositions
	{
		RESERVED = 0, //2 bytes long!
		ADDRESS_EXTENSION = 2,
		ADDRESS = 3
	};
public:
	SetMTAPacket();
	virtual ~SetMTAPacket();
	uint8_t GetAddressExtension();
	void SetAddressExtension(uint8_t AddressExtension);
	uint32_t GetAddress(bool LittleEndian);
	void SetAddress(uint32_t Address, bool LittleEndian);
};

