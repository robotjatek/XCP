#pragma once
#include <cstdint>
#include <vector>

#ifdef XCPLIB_EXPORTS //XCPLIB_EXPORTS macro is defined in the project settings
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif
//TODO: make this work with big endian systems
//TODO: use bitconverter everywhere, instead of converting values by hand every occasion
//TODO: make converting work to the other way as well

namespace XCP
{
	class Bitconverter
	{
	public:
		XCP_API static uint64_t ToUint64(const std::vector<uint8_t>& bytes, bool LittleEndian);
		XCP_API static int64_t ToInt64(const std::vector<uint8_t>& bytes, bool LittleEndian);
		XCP_API static uint32_t ToUint32(const std::vector<uint8_t>& bytes, bool LittleEndian);
		XCP_API static int32_t ToInt32(const std::vector<uint8_t>& bytes, bool LittleEndian);
		XCP_API static uint16_t ToUint16(const std::vector<uint8_t>& bytes, bool LittleEndian);
		XCP_API static int16_t ToInt16(const std::vector<uint8_t>& bytes, bool LittleEndian);
		XCP_API static float ToFloat(const std::vector<uint8_t>& bytes, bool LittleEndian);
		XCP_API static double ToDouble(const std::vector<uint8_t>& bytes, bool LittleEndian);
	};
}