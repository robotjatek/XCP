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

//--------------------------------------------------

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

//--------------------------------------------------

WriteDaqPacket::WriteDaqPacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::WRITE_DAQ;
	m_DataLength = 7;
	m_PacketSize = 8;
	m_Data = new uint8_t[m_DataLength];
}

WriteDaqPacket::~WriteDaqPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void WriteDaqPacket::SetBitOffset(uint8_t Offset)
{
	m_Data[BytePositions::BIT_OFFSET] = Offset;
}

void WriteDaqPacket::SetElementSize(uint8_t Size)
{
	m_Data[BytePositions::SIZE] = Size;
}

void WriteDaqPacket::SetAddressExtension(uint8_t AddressExtension)
{
	m_Data[BytePositions::ADDRESS_EXTENSION] = AddressExtension;
}

void WriteDaqPacket::SetAddress(uint32_t Address, bool LittleEndian)
{
	uint8_t i1, i2, i3, i4;
	i1 = Address & 0xFF;
	i2 = (Address >> 8) & 0xFF;
	i3 = (Address >> 16) & 0xFF;
	i4 = (Address >> 24) & 0xFF;

	if (LittleEndian)
	{
		m_Data[BytePositions::ADDRESS] = i1;
		m_Data[BytePositions::ADDRESS + 1] = i2;
		m_Data[BytePositions::ADDRESS + 2] = i3;
		m_Data[BytePositions::ADDRESS + 3] = i4;
	}
	else
	{
		m_Data[BytePositions::ADDRESS] = i4;
		m_Data[BytePositions::ADDRESS + 1] = i3;
		m_Data[BytePositions::ADDRESS + 2] = i2;
		m_Data[BytePositions::ADDRESS + 3] = i1;
	}
}

//--------------------------------------------------

SetDaqListModePacket::SetDaqListModePacket() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::SET_DAQ_LIST_MODE;
	m_DataLength = 7;
	m_PacketSize = 8;
	m_Data = new uint8_t[m_DataLength];
}

SetDaqListModePacket::SetDaqListModePacket(uint8_t Mode, uint16_t DaqListNumber, uint16_t EventChannel, uint8_t Prescaler, uint8_t Priority, bool LittleEndian) : SetDaqListModePacket()
{
	SetMode(Mode);
	SetDaqListNumber(DaqListNumber,LittleEndian);
	SetEventChannelNumber(EventChannel, LittleEndian);
	SetTransmissionRatePrescaler(Prescaler);
	SetDaqListPriority(Priority);
}

SetDaqListModePacket::~SetDaqListModePacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void SetDaqListModePacket::SetMode(uint8_t Mode)
{
	m_Data[BytePositions::MODE] = Mode;
}

void SetDaqListModePacket::SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian)
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

void SetDaqListModePacket::SetEventChannelNumber(uint16_t EventChannel, bool LittleEndian)
{
	uint8_t t1, t2;
	t1 = EventChannel & 0xFF;
	t2 = (EventChannel >> 8) & 0xFF;

	if (LittleEndian)
	{
		m_Data[BytePositions::EVENT_CHANNEL_NUMBER] = t1;
		m_Data[BytePositions::EVENT_CHANNEL_NUMBER + 1] = t2;
	}
	else
	{
		m_Data[BytePositions::EVENT_CHANNEL_NUMBER] = t2;
		m_Data[BytePositions::EVENT_CHANNEL_NUMBER + 1] = t1;
	}
}

void SetDaqListModePacket::SetTransmissionRatePrescaler(uint8_t Prescaler)
{
	m_Data[BytePositions::TRANSMISSION_RATE_PRESCALER] = Prescaler;
}

void SetDaqListModePacket::SetDaqListPriority(uint8_t Priority)
{
	m_Data[BytePositions::DAQ_LIST_PRIORITY] = Priority;
}

//--------------------------------------------------

StartStopDaqListPacket::StartStopDaqListPacket() : CommandPacket()
{
	m_DataLength = 3;
	m_PacketSize = 4;
	m_Data = new uint8_t[m_DataLength];
	m_PID = CTOMasterToSlaveCommands::START_STOP_DAQ_LIST;
}

StartStopDaqListPacket::StartStopDaqListPacket(uint8_t Mode, uint16_t DaqListNumber, bool LittleEndian) : StartStopDaqListPacket()
{
	SetMode(Mode);
	SetDaqListNumber(DaqListNumber, LittleEndian);
}

StartStopDaqListPacket::~StartStopDaqListPacket()
{
	delete[] m_Data;
}

void StartStopDaqListPacket::SetMode(uint8_t Mode)
{
	m_Data[BytePositions::MODE] = Mode;
}

void StartStopDaqListPacket::SetDaqListNumber(uint16_t DaqListNumber, bool LittleEndian)
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

uint16_t StartStopDaqListPacket::GetDaqListNumber(bool LittleEndian)
{
	uint16_t retval;
	if (LittleEndian)
	{
		retval = m_Data[BytePositions::DAQ_LIST_NUMBER];
		retval |= (((uint16_t)m_Data[BytePositions::DAQ_LIST_NUMBER+1])<<8);
	}
	else
	{
		retval = m_Data[BytePositions::DAQ_LIST_NUMBER+1];
		retval |= (((uint16_t)m_Data[BytePositions::DAQ_LIST_NUMBER]) << 8);
	}
	return retval;
}

//--------------------------------------------------

StartStopDaqListPositiveResponse::StartStopDaqListPositiveResponse(const std::vector<uint8_t>& Data, uint8_t HeaderSize) : ResponsePacket()
{
	m_DataLength = 1;
	m_PacketSize = 2;
	m_Data = new uint8_t[m_DataLength];
	for (unsigned int i = 0; i < m_DataLength; i++)
	{
		m_Data[i] = Data[HeaderSize + i + 1];
	}
}

StartStopDaqListPositiveResponse::~StartStopDaqListPositiveResponse()
{
	delete[] m_Data;
}

uint8_t StartStopDaqListPositiveResponse::GetFirstPid()
{
	return m_Data[BytePositions::FIRST_PID];
}

void StartStopDaqListPositiveResponse::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}

StartStopDaqListPositiveResponse * StartStopDaqListPositiveResponse::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize)
{
	return new StartStopDaqListPositiveResponse(Data,HeaderSize);
}

//--------------------------------------------------

StartStopSynchPacket::StartStopSynchPacket(Mode Mode) : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::START_STOP_SYNCH;
	m_DataLength = 1;
	m_PacketSize = 2;
	m_Data = new uint8_t[m_DataLength];
	SetMode(Mode);
}

StartStopSynchPacket::~StartStopSynchPacket()
{
	delete[] m_Data;
}

void StartStopSynchPacket::SetMode(uint8_t Mode)
{
	m_Data[BytePositions::MODE] = Mode;
}

//--------------------------------------------------

GetDaqProcessorInfo::GetDaqProcessorInfo() : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::GET_DAQ_PROCESSOR_INFO;
	m_DataLength = 0;
	m_Data = nullptr;
	m_PacketSize = 1;
}

GetDaqProcessorInfo::~GetDaqProcessorInfo()
{
}

//--------------------------------------------------

GetDaqProcessorInfoResponse::GetDaqProcessorInfoResponse(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize)
{
	m_PacketSize = 8;
	m_DataLength = 7;
	m_Data = new uint8_t[m_DataLength];
	for (unsigned int i = 0; i < m_DataLength; i++)
	{
		m_Data[i] = Data[HeaderSize + i + 1];
	}
}

GetDaqProcessorInfoResponse::~GetDaqProcessorInfoResponse()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void GetDaqProcessorInfoResponse::Dispatch(IIncomingMessageHandler & Handler)
{
	Handler.Handle(*this);
}

GetDaqProcessorInfoResponse * GetDaqProcessorInfoResponse::Deserialize(const std::vector<uint8_t>& Data, uint8_t HeaderSize, uint8_t TailSize)
{
	return new GetDaqProcessorInfoResponse(Data,HeaderSize, TailSize);
}

uint8_t GetDaqProcessorInfoResponse::GetDaqProperties()
{
	return m_Data[BytePositions::DAQ_PROPERTIES];
}

uint16_t GetDaqProcessorInfoResponse::GetMaxDaq(bool LittleEndian)
{
	if (LittleEndian)
	{
		return (((uint16_t)m_Data[BytePositions::MAX_DAQ + 1]) << 8) | m_Data[BytePositions::MAX_DAQ];
	}
	else
	{
		//do byte-swap
		return (((uint16_t)m_Data[BytePositions::MAX_DAQ]) << 8) | m_Data[BytePositions::MAX_DAQ + 1];
	}
}

uint16_t GetDaqProcessorInfoResponse::GetMaxEventChannel(bool LittleEndian)
{
	if (LittleEndian)
	{
		return (((uint16_t)m_Data[BytePositions::MAX_EVENT_CHANNEL + 1]) << 8) | m_Data[BytePositions::MAX_EVENT_CHANNEL];
	}
	else
	{
		//do byte-swap
		return (((uint16_t)m_Data[BytePositions::MAX_EVENT_CHANNEL]) << 8) | m_Data[BytePositions::MAX_EVENT_CHANNEL + 1];
	}
}

uint8_t GetDaqProcessorInfoResponse::GetMinDaq()
{
	return m_Data[BytePositions::MIN_DAQ];
}

uint8_t GetDaqProcessorInfoResponse::GetDaqKeyByte()
{
	return m_Data[BytePositions::DAQ_KEY_BYTE];
}

//--------------------------------------------------

ClearDAQListPacket::ClearDAQListPacket(uint16_t DAQListNumber, bool LittleEndian) : CommandPacket()
{
	m_PID = CTOMasterToSlaveCommands::CLEAR_DAQ_LIST;
	m_DataLength = 3;
	m_PacketSize = 4;
	m_Data = new uint8_t[m_DataLength];
	m_Data[0] = 0; //Reserved
	SetDaqListNumber(DAQListNumber, LittleEndian);
}

ClearDAQListPacket::~ClearDAQListPacket()
{
	delete[] m_Data;
	m_Data = nullptr;
}

void ClearDAQListPacket::SetDaqListNumber(uint16_t DAQListNumber, bool LittleEndian)
{
	uint8_t t1, t2;
	t1 = DAQListNumber & 0xFF;
	t2 = (DAQListNumber >> 8) & 0xFF;

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

uint16_t ClearDAQListPacket::GetDaqListNumbet(bool LittleEndian)
{
	if (LittleEndian)
	{
		return (((uint16_t)m_Data[BytePositions::DAQ_LIST_NUMBER + 1]) << 8) | m_Data[BytePositions::DAQ_LIST_NUMBER];
	}
	else
	{
		//do byte-swap
		return (((uint16_t)m_Data[BytePositions::DAQ_LIST_NUMBER]) << 8) | m_Data[BytePositions::DAQ_LIST_NUMBER + 1];
	}
}
