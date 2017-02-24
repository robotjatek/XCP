#pragma once
#include <cstdint>
#include <vector>

class IXCPTail
{
public:
	virtual ~IXCPTail() {}
	virtual void Serialize(std::vector<uint8_t>& out_Binary) = 0;
	virtual uint8_t GetSize() = 0;
};
