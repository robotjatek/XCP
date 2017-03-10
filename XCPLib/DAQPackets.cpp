#include "DAQPackets.h"

FreeDaqPacket::FreeDaqPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::FREE_DAQ;
	m_Data = nullptr;
	m_DataLength = 0;
	m_PacketSize = 1;
}

FreeDaqPacket::~FreeDaqPacket()
{
}

//--------------------------------------------------

AllocDaqPacket::AllocDaqPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::ALLOC_DAQ;
	m_Data = new uint8_t[3];
	m_Data[BytePositions::RESERVED] = 0;
	m_DataLength = 3;
	m_PacketSize = 4;
}

AllocDaqPacket::~AllocDaqPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void AllocDaqPacket::SetDaqCount(uint16_t DaqCount, bool LittleEndian)
{
	uint8_t t1, t2;
	t1 = DaqCount & 0xFF;
	t2 = (DaqCount >> 8) & 0xFF;

	if (LittleEndian)
	{
		m_Data[BytePositions::DAQ_COUNT] = t1;
		m_Data[BytePositions::DAQ_COUNT + 1] = t2;
	}
	else
	{
		m_Data[BytePositions::DAQ_COUNT] = t2;
		m_Data[BytePositions::DAQ_COUNT + 1] = t1;
	}
}

uint16_t AllocDaqPacket::GetDaqCount(bool LittleEndian)
{
	uint16_t Retval;
	uint16_t t1, t2;
	t1 = m_Data[BytePositions::DAQ_COUNT];
	t2 = m_Data[BytePositions::DAQ_COUNT + 1];

	if (LittleEndian)
	{
		Retval = (t2 << 8) | t1;
	}
	else
	{
		Retval = (t1 << 8) | t2;
	}

	return Retval;
}

//--------------------------------------------------

AllocOdtPacket::AllocOdtPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::ALLOC_ODT;
	m_Data = new uint8_t[4];
	m_DataLength = 4;
	m_PacketSize = 5;
	m_Data[BytePositions::RESERVED] = 0;
}

AllocOdtPacket::~AllocOdtPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void AllocOdtPacket::SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian)
{
	uint8_t t1, t2;
	t1 = DaqListNumber & 0xFF;
	t2 = (DaqListNumber >> 8) & 0xFF;

	if (LittleEndian)
	{
		m_Data[BytePositions::DAQ_LIST_NUMBER] = t1;
		m_Data[BytePositions::DAQ_LIST_NUMBER + 1] = t2;
	}
	else
	{
		m_Data[BytePositions::DAQ_LIST_NUMBER] = t2;
		m_Data[BytePositions::DAQ_LIST_NUMBER + 1] = t1;
	}
}

void AllocOdtPacket::SetOdtCount(uint8_t OdtCount)
{
	m_Data[BytePositions::ODT_COUNT] = OdtCount;
}

//--------------------------------------------------

AllocOdtEntryPacket::AllocOdtEntryPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::ALLOC_ODT_ENTRY;
	m_Data = new uint8_t[5];
	m_DataLength = 5;
	m_PacketSize = 6;
	m_Data[BytePositions::RESERVED] = 0;
}

AllocOdtEntryPacket::~AllocOdtEntryPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void AllocOdtEntryPacket::SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian)
{
	uint8_t t1, t2;
	t1 = DaqListNumber & 0xFF;
	t2 = (DaqListNumber >> 8) & 0xFF;

	if (LittleEndian)
	{
		m_Data[BytePositions::DAQ_LIST_NUMBER] = t1;
		m_Data[BytePositions::DAQ_LIST_NUMBER + 1] = t2;
	}
	else
	{
		m_Data[BytePositions::DAQ_LIST_NUMBER] = t2;
		m_Data[BytePositions::DAQ_LIST_NUMBER + 1] = t1;
	}
}

void AllocOdtEntryPacket::SetOdtNumber(uint8_t OdtNumber)
{
	m_Data[BytePositions::ODT_NUMBER] = OdtNumber;
}

void AllocOdtEntryPacket::SetOdtEntriesCount(uint8_t OdtEntriesCount)
{
	m_Data[BytePositions::ODT_ENTRIES_COUNT] = OdtEntriesCount;
}

SetDaqPtrPacket::SetDaqPtrPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::SET_DAQ_PTR;
	m_DataLength = 5;
	m_PacketSize = 6;
	m_Data = new uint8_t[m_DataLength];
	m_Data[BytePositions::RESERVED] = 0;
}

SetDaqPtrPacket::~SetDaqPtrPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void SetDaqPtrPacket::SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian)
{
	uint8_t t1, t2;
	t1 = DaqListNumber & 0xFF;
	t2 = (DaqListNumber >> 8) & 0xFF;

	if (LittleEndian)
	{
		m_Data[BytePositions::DAQ_LIST_NUMBER] = t1;
		m_Data[BytePositions::DAQ_LIST_NUMBER + 1] = t2;
	}
	else
	{
		m_Data[BytePositions::DAQ_LIST_NUMBER] = t2;
		m_Data[BytePositions::DAQ_LIST_NUMBER + 1] = t1;
	}
}

void SetDaqPtrPacket::SetOdtNumber(uint8_t OdtNumber)
{
	m_Data[BytePositions::ODT_NUMBER] = OdtNumber;
}

void SetDaqPtrPacket::SetOdtEntryNumber(uint8_t OdtEntryNumber)
{
	m_Data[BytePositions::ODT_ENTRY_NUMBER] = OdtEntryNumber;
}
