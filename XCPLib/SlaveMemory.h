#pragma once
#include <cstdint>
#include <vector>

#ifdef XCPLIB_EXPORTS //XCPLIB_EXPORTS macro is defined in the project settings
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif

class ODTEntry
{
private:
	uint32_t m_Address;
	uint8_t m_AddressExtension;
	uint8_t m_Length;
public:
	XCP_API ODTEntry();
	XCP_API ODTEntry(uint32_t Address, uint8_t AddressExtension, uint8_t Length);
	XCP_API virtual ~ODTEntry();
	XCP_API const uint32_t GetAddress() const;
	XCP_API void SetAddress(uint32_t Address);
	XCP_API const uint8_t GetAddressExtension() const;
	XCP_API void SetAddressExtension(uint8_t AddressExtension);
	XCP_API const uint8_t GetLength() const;
	XCP_API void SetLength(uint8_t Length);
};

class ODT
{
private:
	std::vector<ODTEntry> m_EntryList;
	bool m_First;
public:
	XCP_API ODT();
	XCP_API virtual ~ODT();
	XCP_API void AddEntry(ODTEntry Entry);
	XCP_API ODTEntry GetEntry(uint32_t Index);
	XCP_API const uint32_t GetODTSize() const;
	XCP_API const uint32_t GetNumberOfEntries() const;
	XCP_API const bool IsFirst() const;
	XCP_API void SetFirst(bool First);
};

class DAQ
{
private:
	std::vector<ODT> m_ODTList;
	uint8_t m_Mode;
	uint16_t m_EventChannel;
	uint8_t m_Prescaler;
	uint8_t m_Priority;
	uint8_t m_FirstPid;
public:
	XCP_API DAQ();
	XCP_API virtual ~DAQ();
	XCP_API void AddODT(ODT Odt);
	XCP_API ODT GetOdt(uint32_t Index);
	XCP_API void SetMode(uint8_t Mode);
	XCP_API const uint8_t GetMode() const;
	XCP_API const uint16_t GetEventChannel() const;
	XCP_API const uint8_t GetPrescaler() const;
	XCP_API const uint8_t GetPriority() const;
	XCP_API void SetEventChannel(uint16_t EventChannel);
	XCP_API void SetPrescaler(uint8_t Prescaler);
	XCP_API void SetPriority(uint8_t Priority);
	XCP_API const uint16_t GetNumberOfODTs() const;
	XCP_API uint8_t GetFirstPid();
	XCP_API void SetFirstPid(uint8_t FirstPid);
};

//TODO: write an iterator for this class

class DAQLayout
{
private:
	std::vector<DAQ> m_DAQList;
	bool Initialized = false;
public:
	XCP_API DAQLayout();
	XCP_API virtual ~DAQLayout();
	XCP_API void AddDAQ(DAQ daq);
	XCP_API DAQ GetDAQ(uint32_t Index);
	XCP_API const uint16_t GetNumberOfDAQLists() const;
	XCP_API int32_t CalculateDAQNumberFromAbsolutePID(uint8_t PID);
	XCP_API int32_t CalculateODTNumberFromAbsolutePID(uint8_t PID);
	XCP_API ODT GetODTFromAbsolutePID(uint8_t PID);
	XCP_API bool IsInitialized();
	XCP_API void SetInitialized(bool i);
};