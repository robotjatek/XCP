#include "Utils.h"


uint64_t XCP::Bitconverter::ToUint64(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	if (bytes.size() == 8)
	{
		if (LittleEndian)
		{
			return (((uint64_t)bytes[7]) << 56) | (((uint64_t)bytes[6]) << 48) | (((uint64_t)bytes[5]) << 40) | (((uint64_t)bytes[4])<<32) |
				(((uint64_t)bytes[3]) << 24) | (((uint64_t)bytes[2]) << 16) | (((uint64_t)bytes[1]) << 8) | bytes[0];
		}
		else
		{
			//do byte-swap
			return (((uint64_t)bytes[0]) << 56) | (((uint64_t)bytes[1]) << 48) | (((uint64_t)bytes[2]) << 40) | (((uint64_t)bytes[3]) << 32) |
				(((uint64_t)bytes[4]) << 24) | (((uint64_t)bytes[5]) << 16) | (((uint64_t)bytes[6]) << 8) | bytes[7];
		}
	}
	else
	{
		return 0;
	}
}

int64_t XCP::Bitconverter::ToInt64(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	if (bytes.size() == 8)
	{
		if (LittleEndian)
		{
			return (int64_t)((((uint64_t)bytes[7]) << 56) | (((uint64_t)bytes[6]) << 48) | (((uint64_t)bytes[5]) << 40) | (((uint64_t)bytes[4]) << 32) |
				(((uint64_t)bytes[3]) << 24) | (((uint64_t)bytes[2]) << 16) | (((uint64_t)bytes[1]) << 8) | bytes[0]);
		}
		else
		{
			//do byte-swap
			return (int64_t)((((uint64_t)bytes[0]) << 56) | (((uint64_t)bytes[1]) << 48) | (((uint64_t)bytes[2]) << 40) | (((uint64_t)bytes[3]) << 32) |
				(((uint64_t)bytes[4]) << 24) | (((uint64_t)bytes[5]) << 16) | (((uint64_t)bytes[6]) << 8) | bytes[7]);
		}
	}
	else
	{
		return 0;
	}
}

uint32_t XCP::Bitconverter::ToUint32(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	if (bytes.size() == 4)
	{
		if (LittleEndian)
		{
			return  (((uint32_t)bytes[3]) << 24) | (((uint32_t)bytes[2]) << 16) | (((uint32_t)bytes[1]) << 8) | bytes[0];
		}
		else
		{
			//do byte-swap
			return (((uint32_t)bytes[0]) << 24) | (((uint32_t)bytes[1]) << 16) | (((uint32_t)bytes[2]) << 8) | bytes[3];
		}
	}
	else
	{
		return 0;
	}
}

int32_t XCP::Bitconverter::ToInt32(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	if (bytes.size() == 4)
	{
		if (LittleEndian)
		{
			return  (int32_t)((((uint32_t)bytes[3]) << 24) | (((uint32_t)bytes[2]) << 16) | (((uint32_t)bytes[1]) << 8) | bytes[0]);
		}
		else
		{
			//do byte-swap
			return (int32_t)((((uint32_t)bytes[0]) << 24) | (((uint32_t)bytes[1]) << 16) | (((uint32_t)bytes[2]) << 8) | bytes[3]);
		}
	}
	else
	{
		return 0;
	}
}

uint16_t XCP::Bitconverter::ToUint16(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	if (bytes.size() == 2)
	{
		if (LittleEndian)
		{
			return  (((uint16_t)bytes[1]) << 8) | bytes[0];
		}
		else
		{
			//do byte-swap
			return ((((uint16_t)bytes[0]) << 8) | ((uint32_t)bytes[1]));
		}
	}
	else
	{
		return 0;
	}
}

int16_t XCP::Bitconverter::ToInt16(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	if (bytes.size() == 2)
	{
		if (LittleEndian)
		{
			return  (int16_t)((((uint16_t)bytes[1]) << 8) | bytes[0]);
		}
		else
		{
			//do byte-swap
			return (int16_t)((((uint16_t)bytes[0]) << 8) | ((uint32_t)bytes[1]));
		}
	}
	else
	{
		return 0;
	}
}

float XCP::Bitconverter::ToFloat(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	throw - 1;
	return 0.0f;
}

double XCP::Bitconverter::ToDouble(const std::vector<uint8_t>& bytes, bool LittleEndian)
{
	throw - 1;
	return 0.0;
}
