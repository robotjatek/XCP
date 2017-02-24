#pragma once
#include <cstdint>
#include <vector>

class IXCPHeader
{
public:
	virtual ~IXCPHeader(){}
	virtual void Serialize(std::vector<uint8_t>& out_Binary) = 0;
	virtual uint8_t GetSize() = 0;
};