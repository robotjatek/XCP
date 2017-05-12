#include "IncomingHandlerExternal.h"
#include "XCPWorkerThread.h"

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
	//ODT& odt = m_MasterPtr->GetDaqLayout().GetDAQ(Packet.GetDAQIndex()).GetOdt(Packet.GetODTIndex());
	for (uint32_t i = 0; i <  m_MasterPtr->GetDaqLayout().GetDAQ(Packet.GetDAQIndex()).GetOdt(Packet.GetODTIndex()).GetNumberOfEntries(); i++)
	{
		ODTEntry& entry = m_MasterPtr->GetDaqLayout().GetDAQ(Packet.GetDAQIndex()).GetOdt(Packet.GetODTIndex()).GetEntry(i);
		m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), i, Packet.GetTimestamp(), (int8_t)Packet.GetByteElement(i));
	}
	if (m_FirstMeasurementData)
	{
		m_wnd->FirstMeasurementArrived(Packet.GetTimestamp());
		m_FirstMeasurementData = false;
	}
	//m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), 1, x, (uint8_t)Packet.GetByteElement(1));
	x++;
}

void IncomingHandlerExternal::Reset()
{
	x = 0;
	m_FirstMeasurementData = true;
}
