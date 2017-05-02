#include "IncomingHandlerExternal.h"
#include "XCPWorkerThread.h"

IncomingHandlerExternal::IncomingHandlerExternal(XCPWorkerThread* wnd)
{
	m_wnd = wnd;
	Reset();
}

IncomingHandlerExternal::~IncomingHandlerExternal()
{
}

void IncomingHandlerExternal::Handle(DTO & Packet)
{
	m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), 0, x, (int8_t)Packet.GetByteElement(0));
	//m_wnd->AddPointToSeries(Packet.GetDAQIndex(), Packet.GetODTIndex(), 1, x, (uint8_t)Packet.GetByteElement(1));
	x++;
}

void IncomingHandlerExternal::Reset()
{
	x = 0;
}
