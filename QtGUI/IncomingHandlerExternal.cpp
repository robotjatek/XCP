#include "IncomingHandlerExternal.h"
#include "XCPWorkerThread.h"

IncomingHandlerExternal::IncomingHandlerExternal(XCPWorkerThread* wnd)
{
	m_wnd = wnd;
}

IncomingHandlerExternal::~IncomingHandlerExternal()
{
}

void IncomingHandlerExternal::Handle(DTO & Packet)
{
//	for (uint32_t i = 0; i < Packet.GetDataLength(); i++)
	{
		//std::cout << std::hex << (int)Packet.GetByteElement(i) << " ";
		m_wnd->AddPoint(0, (int8_t)Packet.GetByteElement(0));
		m_wnd->AddPoint(1, (uint8_t)Packet.GetByteElement(1));
	}
}
