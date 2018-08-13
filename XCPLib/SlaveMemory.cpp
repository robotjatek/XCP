#include "SlaveMemory.h"

ODTEntry::ODTEntry()
{
	m_Address = 0;
	m_AddressExtension = 0;
	m_Length = 0;
	m_DataType = 0;
}

ODTEntry::ODTEntry(uint32_t Address, uint8_t AddressExtension, uint8_t Length)
{
	m_Address = Address;
	m_AddressExtension = AddressExtension;
	m_Length = Length;
}

ODTEntry::ODTEntry(const ODTEntry & o)
{
	m_Address = o.m_Address;
	m_AddressExtension = o.m_AddressExtension;
	m_Length = o.m_Length;
	m_DataType = o.m_DataType;
}

ODTEntry::~ODTEntry()
{
}

const uint32_t ODTEntry::GetAddress() const
{
	return m_Address;
}

void ODTEntry::SetAddress(uint32_t Address)
{
	m_Address = Address;
}

const uint8_t ODTEntry::GetAddressExtension() const
{
	return m_AddressExtension;
}

void ODTEntry::SetAddressExtension(uint8_t AddressExtension)
{
	m_AddressExtension = AddressExtension;
}

const uint8_t ODTEntry::GetLength() const
{
	return m_Length;
}

void ODTEntry::SetLength(uint8_t Length)
{
	m_Length = Length;
}

void ODTEntry::SetDataType(uint8_t DataType)
{
	m_DataType = DataType;
	switch (m_DataType)
	{
	case MeasurementDataTypes::XBYTE:
		m_Length = 1;
		break;
	case MeasurementDataTypes::XUBYTE:
		m_Length = 1;
		break;
	case MeasurementDataTypes::XSHORT:
		m_Length = 2;
		break;
	case MeasurementDataTypes::XUSHORT:
		m_Length = 2;
		break;
	case MeasurementDataTypes::XWORD:
		m_Length = 4;
		break;
	case MeasurementDataTypes::XUWORD:
		m_Length = 4;
		break;
	case MeasurementDataTypes::XLONG:
		m_Length = 8;
		break;
	case MeasurementDataTypes::XULONG:
		m_Length = 8;
		break;
	case MeasurementDataTypes::XFLOAT32:
		m_Length = 4;
		break;
	case MeasurementDataTypes::XFLOAT64:
		m_Length = 8;
		break;
	default:
		m_Length = 0; //undefined data type = error
		break;
	}
}

const uint8_t ODTEntry::GetDataType() const
{
	return m_DataType;
}

ODT::ODT()
{
	m_First = false;
}


ODT::ODT(const ODT & o) : m_EntryList(o.m_EntryList)
{
	m_First = o.m_First;
}

ODT::~ODT()
{
}

void ODT::AddEntry(ODTEntry Entry)
{
	m_EntryList.push_back(Entry);
}

ODTEntry& ODT::GetEntry(uint32_t Index)
{
	return m_EntryList[Index];
}

const uint32_t ODT::GetODTSize() const
{
	uint32_t Size = 0;
	for (auto a : m_EntryList)
	{
		Size += a.GetLength();
	}
	return Size;
}

XCP_API const uint32_t ODT::GetNumberOfEntries() const
{
	return (uint32_t)m_EntryList.size();
}

XCP_API const bool ODT::IsFirst() const
{
	return m_First;
}

XCP_API void ODT::SetFirst(bool First)
{
	m_First = First;
}

DAQ::DAQ()
{
}

DAQ::DAQ(const DAQ & o) : m_ODTList(o.m_ODTList)
{
	m_Mode = o.m_Mode;
	m_EventChannel = o.m_EventChannel;
	m_Prescaler = o.m_Prescaler;
	m_Priority = o.m_Priority;
	m_FirstPid = o.m_FirstPid;
}

DAQ::~DAQ()
{
}

void DAQ::AddODT(ODT Odt)
{
	if (m_ODTList.size() == 0)
	{
		Odt.SetFirst(true);
	}
	m_ODTList.push_back(Odt);
}

ODT& DAQ::GetOdt(uint32_t Index)
{
	return m_ODTList[Index];
}

void DAQ::SetMode(uint8_t Mode)
{
	m_Mode = Mode;
}

const uint8_t DAQ::GetMode() const
{
	return m_Mode;
}

const uint16_t DAQ::GetEventChannel() const
{
	return m_EventChannel;
}

const uint8_t DAQ::GetPrescaler() const
{
	return m_Prescaler;
}

const uint8_t DAQ::GetPriority() const
{
	return m_Priority;
}

void DAQ::SetEventChannel(uint16_t EventChannel)
{
	m_EventChannel = EventChannel;
}

void DAQ::SetPrescaler(uint8_t Prescaler)
{
	m_Prescaler = Prescaler;
}

void DAQ::SetPriority(uint8_t Priority)
{
	m_Priority = Priority;
}

XCP_API const uint16_t DAQ::GetNumberOfODTs() const
{
	return (uint16_t)m_ODTList.size();
}

XCP_API uint8_t DAQ::GetFirstPid()
{
	return m_FirstPid;
}

XCP_API void DAQ::SetFirstPid(uint8_t FirstPid)
{
	m_FirstPid = FirstPid;
}

XCP_API void DAQ::SetLastTimestamp(uint32_t Timestamp)
{
	m_LastTimestamp = Timestamp;
}

XCP_API uint32_t DAQ::GetLastTimestamp()
{
	return m_LastTimestamp;
}

DAQLayout::DAQLayout()
{
}

DAQLayout::DAQLayout(const DAQLayout & o)  : m_DAQList(o.m_DAQList)
{
	Initialized = o.Initialized;
}

DAQLayout::~DAQLayout()
{
}

void DAQLayout::AddDAQ(DAQ daq)
{
	m_DAQList.push_back(daq);
}

XCP_API void DAQLayout::SetDAQ(unsigned int id, DAQ daq)
{
	if (id < m_DAQList.size())
	{
		m_DAQList[id] = daq;
	}
}

DAQ& DAQLayout::GetDAQ(uint32_t Index)
{
	return m_DAQList[Index];
}

XCP_API const uint16_t DAQLayout::GetNumberOfDAQLists() const
{
	return (uint16_t)m_DAQList.size();
}

XCP_API int32_t DAQLayout::CalculateDAQNumberFromAbsolutePID(uint8_t PID)
{
	bool found = false;
	int i;
	for (i = 0; i < GetNumberOfDAQLists(); i++)
	{
		uint8_t t1 = GetDAQ(i).GetFirstPid();
		for (int j = 0; j < GetDAQ(i).GetNumberOfODTs(); j++)
		{
			if (t1 == PID)
			{
				found = true;
				break;
			}
			t1++;
		}
		if (found)
		{
			break;
		}
	}

	return i-1;
}

XCP_API int32_t DAQLayout::CalculateODTNumberFromAbsolutePID(uint8_t PID)
{
	int i;
	for (i = 0; i < GetNumberOfDAQLists(); i++)
	{
		uint8_t t1 = GetDAQ(i).GetFirstPid();
		for (int j = 0; j < GetDAQ(i).GetNumberOfODTs(); j++)
		{
			if (t1 == PID)
			{
				return j;
			}
			t1++;
		}
	}

	return -1;
}

XCP_API ODT& DAQLayout::GetODTFromAbsolutePID(uint8_t PID)
{
	bool found = false;
	int i;
	for (i = 0; i < GetNumberOfDAQLists(); i++)
	{
		uint8_t t1 = GetDAQ(i).GetFirstPid();
		for (int j = 0; j < GetDAQ(i).GetNumberOfODTs(); j++)
		{
			if (t1 == PID)
			{
				return GetDAQ(i).GetOdt(j);
			}
			t1++;
		}
	}

	throw "Not found";
}

XCP_API bool DAQLayout::IsInitialized()
{
	return Initialized;
}

XCP_API void DAQLayout::SetInitialized(bool i)
{
	Initialized = i;
}

XCP_API DAQLayout& DAQLayout::WithDAQ(std::function<DAQ&(DAQ&)> daq_func)
{
	DAQ d;
	this->AddDAQ(daq_func(d));

	return *this;
}

XCP_API DAQ& DAQ::WithODT(std::function<ODT&(ODT&)> odt_func)
{
	ODT o;
	this->AddODT(odt_func(o));

	return *this;
}

XCP_API ODT& ODT::WithODTEntry(std::function<ODTEntry&(ODTEntry&)> entry_func)
{
	ODTEntry e;
	this->AddEntry(entry_func(e));

	return *this;
}
