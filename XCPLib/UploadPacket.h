#pragma once
#include "CommandPacket.h"
#include "ResponsePacket.h"

//Upload from slave to master
class UploadPacket final :
	public CommandPacket
{
public:
	//Number of dataelements: 1..[MAXCTO/AG - 1] in standardmode, 1..255 in block mode
	UploadPacket(uint8_t NumberOfDataElements);
	virtual ~UploadPacket();
	void SetNumberOfDataElements(uint8_t num);
	uint8_t GetNumberOfDataElements();
};

//------------------------------------------------------------------------------------------------------------------------------------------------------

class UploadResponse final : public ResponsePacket
{
private:
	uint8_t m_AG;
	uint8_t m_ElementSizeInBytes;
	uint8_t m_NumberOfElements;
	UploadResponse(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t AG);
public:
	~UploadResponse();
	static UploadResponse* Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t AG);
	uint8_t GetNumberOfElements();
	template<typename T>
	T GetElement(uint8_t id, bool LittleEndian) = delete;
	template<>
	uint8_t GetElement<uint8_t>(uint8_t id, bool LittleEndian) { return m_Data[id]; };

	template<>
	uint16_t GetElement<uint16_t>(uint8_t id, bool LittleEndian)
	{
		uint16_t t1, t2;
		t1 = m_Data[1 + id * 2];
		t2 = m_Data[1 + id * 2 + 1];

		if (LittleEndian)
		{
			return ((t2) << 8) | t1;
		}
		else
		{
			//do byte-swap
			return ((t1) << 8) | t2;
		}
	};

	template<>
	uint32_t GetElement<uint32_t>(uint8_t id, bool LittleEndian)
	{
		if (LittleEndian)
		{
			return  (((uint32_t)m_Data[3 + id * 4 + 3]) << 24) | (((uint32_t)m_Data[3 + id * 4 + 2]) << 16) | (((uint32_t)m_Data[3 + id * 4 + 1]) << 8) | m_Data[3 + id * 4];
		}
		else
		{
			//do byte-swap
			return (((uint32_t)m_Data[3 + id * 4]) << 24) | (((uint32_t)m_Data[3 + id * 4 + 1]) << 16) | (((uint32_t)m_Data[3 + id * 4 + 2]) << 8) | m_Data[3 + id * 4 + 3];
		}
	};

	virtual void Dispatch(IIncomingMessageHandler& Handler);
};

//-------------------------------------------------------------------------------------------------------------------------

class ShortUploadPacket final : public CommandPacket
{
	enum BytePositions
	{
		NUMBER_OF_DATA_ELEMENTS = 0,
		RESERVED = 1,
		ADDRESS_EXTENSION = 2,
		ADDRESS = 3, //4 bytes!
	};
public:
	ShortUploadPacket();
	virtual ~ShortUploadPacket();
	void SetAddress(uint32_t Address, bool LittleEndian);
	uint32_t GetAddress(bool LittleEndian);
	uint8_t GetAddressExtension();
	void SetAddressExtension(uint8_t AddressExtension);
	void SetNumberOfDataElements(uint8_t NumberOfDataElements);
	uint8_t GetNumberOfDataElements();
};