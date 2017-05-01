#include "XCPWorkerThread.h"
#include <XCPMaster.h>
#include <qtcpsocket.h>
#include <vector>

XCPWorkerThread::XCPWorkerThread(XCPMaster* master)
	: QThread()
{
	this->master = master;

}

XCPWorkerThread::~XCPWorkerThread()
{
	//delete s;
}

void XCPWorkerThread::run()
{
	TestXCP();
}

void XCPWorkerThread::AddPoint(unsigned int series, double point)
{
	//emit NotifyUI(series, point);
}

void XCPWorkerThread::AddPointToSeries(uint16_t DAQId, uint8_t ODTId, uint32_t EntryId, double x, double y)
{
	emit NotifyUI(DAQId, ODTId, EntryId, x, y);
}

void XCPWorkerThread::TestXCP()
{
	s = new QTcpSocket(0);

	//	connect(s, SIGNAL(connected()), this, SLOT(connected()));
	//	connect(s, SIGNAL(disconnected()), this, SLOT(disconnected()));
	//	connect(s, SIGNAL(readyRead()), this, SLOT(readyRead()));
	s->connectToHost("127.0.0.1", 5555);
	s->waitForConnected(-1);

	XCPMsgPtr connect_message = master->CreateConnectMessage(ConnectPacket::ConnectMode::NORMAL);

	Send(s, std::move(connect_message));
	MaxRecvsize = master->GetSlaveProperties().MaxDto;

	XCPMsgPtr GetStatus = master->CreateGetStatusMessage();
	Send(s, std::move(GetStatus));

	XCPMsgPtr Synch = master->CreateSynchMessage();
	Send(s, std::move(Synch));

	XCPMsgPtr GetSeed1 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::DAQ);
	Send(s, std::move(GetSeed1));
	std::vector<XCPMsgPtr> UnlockMessages = master->CreateUnlockMessages();
	Send(s, std::move(UnlockMessages[0]));

	XCPMsgPtr GetSeed2 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::CAL_PG);
	Send(s, std::move(GetSeed2));
	std::vector<XCPMsgPtr> UnlockMessages1 = master->CreateUnlockMessages();
	Send(s, std::move(UnlockMessages1[0]));

	XCPMsgPtr GetSeed3 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::PGM);
	Send(s, std::move(GetSeed3));
	std::vector<XCPMsgPtr> UnlockMessages2 = master->CreateUnlockMessages();
	Send(s, std::move(UnlockMessages2[0]));

	XCPMsgPtr GetSeed4 = master->CreateGetSeedMessage(GetSeedPacket::Mode::FIRST_PART, GetSeedPacket::Resource::STIM);
	Send(s, std::move(GetSeed4));
	std::vector<XCPMsgPtr> UnlockMessages4 = master->CreateUnlockMessages();
	Send(s, std::move(UnlockMessages4[0]));

	//XCPMsgPtr GetSeed2 = master.CreateGetSeedMessage(GetSeedPacket::Mode::REMAINING_PART, GetSeedPacket::Resource::DAQ);
	//Send(s, std::move(GetSeed2));

	XCPMsgPtr SetMTA = master->CreateSetMTAMessage(0x219020, 0);
	Send(s, std::move(SetMTA));
	//XCPMsgPtr SetMTA = master.CreateSetMTAMessage(0x0, 0);

	XCPMsgPtr Upload = master->CreateUploadMessage(10);
	Send(s, std::move(Upload));

	XCPMsgPtr ShortUpload = master->CreateShortUploadMessage(10, 0x21A08D, 0);
	Send(s, std::move(ShortUpload));

	XCPMsgPtr GetDaqProcessorInfo = master->CreateGetDaqProcessorInfoMessage();
	Send(s, std::move(GetDaqProcessorInfo));

	XCPMsgPtr FreeDaq = master->CreateFreeDaqMessage();
	Send(s, std::move(FreeDaq));

	//Using the low level DAQ APIs, it is the programmer's responsibilty to maintain the DAQ layout descriptor:
	using ModeFieldBits = SetDaqListModePacket::ModeFieldBits;
	/*DAQLayout daqlayout;
	DAQ daq0;
	ODT daq0odt0;
	ODTEntry daq0odt0entry0(0x21A1BD, 0, 1);
	ODTEntry daq0odt0entry1(0x21A08D, 0, 1);
	daq0odt0.AddEntry(daq0odt0entry0);
	daq0odt0.AddEntry(daq0odt0entry1);
	daq0.AddODT(daq0odt0);
	daq0.SetEventChannel(0);
	daq0.SetMode(ModeFieldBits::TIMESTAMP);
	daq0.SetPrescaler(1);
	daq0.SetPriority(1);
	daqlayout.AddDAQ(daq0);
	master->SetDaqLayout(daqlayout);*/

	/*XCPMsgPtr AllocDaq = master->CreateAllocDaqMessage(1);
	Send(s, std::move(AllocDaq));*/

	//XCPMsgPtr AllocOdt = master->CreateAllocOdtMessage(0, 1);
	//Send(s, std::move(AllocOdt));

	/*XCPMsgPtr AllocOdtEntry = master->CreateAllocOdtEntryMessage(0, 0, 2);
	Send(s, std::move(AllocOdtEntry));*/

	/*XCPMsgPtr SetDaqPtr1 = master->CreateSetDaqPtrMessage(0, 0, 0);
	Send(s, std::move(SetDaqPtr1));*/

	//XCPMsgPtr WriteDaq1 = master->CreateWriteDaqMessage(0xFF, 1, 0, 0x21A1BD); //sbyte triangle signal
	//Send(s, std::move(WriteDaq1));

	//XCPMsgPtr SetDaqPtr2 = master->CreateSetDaqPtrMessage(0, 0, 1);
	//Send(s, std::move(SetDaqPtr2));

	//XCPMsgPtr WriteDaq2 = master->CreateWriteDaqMessage(0xFF, 1, 0, 0x21A08D); //ubyte square signal
	//Send(s, std::move(WriteDaq2));


	if (master->GetDaqLayout().IsInitialized())
	{
		Send(s, std::move(master->CreateAllocDaqMessage(master->GetDaqLayout().GetNumberOfDAQLists())));

		for (int i = 0; i < master->GetDaqLayout().GetNumberOfDAQLists(); i++)
		{
			DAQ daq = master->GetDaqLayout().GetDAQ(i);
			Send(s, std::move(master->CreateAllocOdtMessage(i, daq.GetNumberOfODTs())));
			for (int j = 0; j < master->GetDaqLayout().GetDAQ(i).GetNumberOfODTs(); j++)
			{
				ODT odt = daq.GetOdt(j);
				Send(s, std::move(master->CreateAllocOdtEntryMessage(i, j, odt.GetNumberOfEntries())));
				for (int k = 0; k < master->GetDaqLayout().GetDAQ(i).GetOdt(j).GetNumberOfEntries(); k++)
				{
					ODTEntry entry = odt.GetEntry(k);
					Send(s, std::move(master->CreateSetDaqPtrMessage(i, j, k)));
					Send(s, std::move(master->CreateWriteDaqMessage(0xFF, entry.GetLength(), entry.GetAddressExtension(), entry.GetAddress())));
				}
			}
		}

		XCPMsgPtr SetDaqListMode = master->CreateSetDaqListModeMessage(ModeFieldBits::TIMESTAMP, 0, 1, 1, 1); //DAQ direction; Timestamp on; do not use ctr field; Disabled alternating display; Transmit DTO WITH identification field;
		Send(s, std::move(SetDaqListMode));

		XCPMsgPtr StartStopDaqList = master->CreateStartStopDaqListMessage(StartStopDaqListPacket::Mode::SELECT, 0);
		Send(s, std::move(StartStopDaqList));

		XCPMsgPtr StartStopSynch = master->CreateStartStopSynchMessage(StartStopSynchPacket::Mode::START_SELECTED);
		Send(s, std::move(StartStopSynch));

		int i = 0;
		while (i < 1000)
		{
			if (s->waitForReadyRead(-1))
			{
				bytes.resize(MaxRecvsize);
				int recv_size = s->read((char*)&bytes[0], MaxRecvsize);
				bytes.resize(recv_size);
				for (int i = 0; i < recv_size; i++)
				{
					std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
				}
				std::cout << "\n";
				while (bytes.size())
				{
					XCPMsgPtr asd = master->DeserializeMessage(bytes);

				}
				//bytes.clear();
				i++;
			}
		}

		XCPMsgPtr Stop = master->CreateStartStopSynchMessage(StartStopSynchPacket::Mode::STOP_ALL);
		Send(s, std::move(Stop));
	}

	XCPMsgPtr disconnect_message = master->CreateDisconnectMessage();
	Send(s, std::move(disconnect_message));
	emit Finished();
	delete s;
}

void XCPWorkerThread::Send(QTcpSocket* s, XCPMsgPtr message)
{
	message->Serialize(bytes);
	s->write((const char*)bytes.begin()._Ptr, bytes.size());
	bytes.clear();
	bytes.resize(MaxRecvsize);
	master->AddSentMessage(message.get());
	s->flush();

	if (s->waitForReadyRead(-1))
	{
		int recv_size = s->read((char*)&bytes[0], MaxRecvsize);
		bytes.resize(recv_size);
		for (int i = 0; i < recv_size; i++)
		{
			std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
		}
		std::cout << "\n";
		XCPMsgPtr asd = master->DeserializeMessage(bytes);
	}
}