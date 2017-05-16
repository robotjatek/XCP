#include "IncomingHandlerExternal.h"
#include "XCPWorkerThread.h"
#include <Utils.h>

IncomingHandlerExternal::IncomingHandlerExternal(XCPWorkerThread* wnd, XCPMaster* master)
{
	m_wnd = wnd;
	m_MasterPtr = master;
	Reset();
}

IncomingHandlerExternal::~IncomingHandlerExternal()
{
}

void IncomingHandlerExternal::Handle(DTO & Packet)
{
	ODT o = m_MasterPtr->GetDaqLayout().GetDAQ(Packet.GetDAQIndex()).GetOdt(Packet.GetODTIndex());
	uint32_t ByteIndex = 0;
	for (uint32_t i = 0; i <  o.GetNumberOfEntries(); i++)
	{
		uint32_t LastTimestamp = m_MasterPtr->GetDaqLayout().GetDAQ(Packet.GetDAQIndex()).GetLastTimestamp();
		switch (o.GetEntry(i).GetDataType())
		{
		case MeasurementDataTypes::XBYTE:
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, (int8_t)Packet.GetByteElement(ByteIndex));
			break;
		case MeasurementDataTypes::XUBYTE:
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, (uint8_t)Packet.GetByteElement(ByteIndex));
			break;
		case MeasurementDataTypes::XSHORT:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex+1));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToInt16(vec,m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		case MeasurementDataTypes::XUSHORT:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex + 1));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToUint16(vec, m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		case MeasurementDataTypes::XWORD:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex + 1));
			vec.push_back(Packet.GetByteElement(ByteIndex + 2));
			vec.push_back(Packet.GetByteElement(ByteIndex + 3));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToInt32(vec, m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		case MeasurementDataTypes::XUWORD:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex + 1));
			vec.push_back(Packet.GetByteElement(ByteIndex + 2));
			vec.push_back(Packet.GetByteElement(ByteIndex + 3));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToUint32(vec, m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		case MeasurementDataTypes::XLONG:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex + 1));
			vec.push_back(Packet.GetByteElement(ByteIndex + 2));
			vec.push_back(Packet.GetByteElement(ByteIndex + 3));
			vec.push_back(Packet.GetByteElement(ByteIndex + 4));
			vec.push_back(Packet.GetByteElement(ByteIndex + 5));
			vec.push_back(Packet.GetByteElement(ByteIndex + 6));
			vec.push_back(Packet.GetByteElement(ByteIndex + 7));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToInt64(vec, m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		case MeasurementDataTypes::XULONG:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex + 1));
			vec.push_back(Packet.GetByteElement(ByteIndex + 2));
			vec.push_back(Packet.GetByteElement(ByteIndex + 3));
			vec.push_back(Packet.GetByteElement(ByteIndex + 4));
			vec.push_back(Packet.GetByteElement(ByteIndex + 5));
			vec.push_back(Packet.GetByteElement(ByteIndex + 6));
			vec.push_back(Packet.GetByteElement(ByteIndex + 7));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToUint64(vec, m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		case MeasurementDataTypes::XFLOAT32:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex + 1));
			vec.push_back(Packet.GetByteElement(ByteIndex + 2));
			vec.push_back(Packet.GetByteElement(ByteIndex + 3));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToFloat(vec, m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		case MeasurementDataTypes::XFLOAT64:
		{
			std::vector<uint8_t> vec;
			vec.push_back(Packet.GetByteElement(ByteIndex));
			vec.push_back(Packet.GetByteElement(ByteIndex + 1));
			vec.push_back(Packet.GetByteElement(ByteIndex + 2));
			vec.push_back(Packet.GetByteElement(ByteIndex + 3));
			vec.push_back(Packet.GetByteElement(ByteIndex + 4));
			vec.push_back(Packet.GetByteElement(ByteIndex + 5));
			vec.push_back(Packet.GetByteElement(ByteIndex + 6));
			vec.push_back(Packet.GetByteElement(ByteIndex + 7));
			m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, LastTimestamp, XCP::Bitconverter::ToDouble(vec, m_MasterPtr->GetSlaveProperties().ByteOrder == 0));
		}
			break;
		}
		ByteIndex += o.GetEntry(i).GetLength();
	}
	if (m_FirstMeasurementData)
	{
		m_wnd->FirstMeasurementArrived(Packet.GetTimestamp());
		m_FirstMeasurementData = false;
	}
	x++;
}

void IncomingHandlerExternal::Reset()
{
	x = 0;
	m_FirstMeasurementData = true;
}
