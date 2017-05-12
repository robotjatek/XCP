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
	ODT& odt = m_MasterPtr->GetDaqLayout().GetDAQ(Packet.GetDAQIndex()).GetOdt(Packet.GetODTIndex());
	for (int i = 0; i < odt.GetNumberOfEntries(); i++)
	{
		//ODTEntry& entry = odt.GetEntry(i);
		
	}
	if (m_FirstMeasurementData)
	{
		m_wnd->FirstMeasurementArrived(Packet.GetTimestamp());
		m_FirstMeasurementData = false;
	}

	printf("%d\n", Packet.GetTimestamp());
	m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), 0, Packet.GetTimestamp(), (int8_t)Packet.GetByteElement(0));
	//m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), 1, x, (uint8_t)Packet.GetByteElement(1));
	x++;
}

void IncomingHandlerExternal::Reset()
{
	x = 0;
	m_FirstMeasurementData = true;
}
