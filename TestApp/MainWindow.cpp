#include "MainWindow.h"

#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib, "wxbase31ud.lib")
#else
#pragma comment(lib, "wxbase31u.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "wxbase31d.lib")
#else
#pragma comment(lib, "wxbase31.lib")
#endif
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef XCPLIB_EXPORTS
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif

#include <Windows.h>
#include <tchar.h>
//#include <WinSock2.h>
//#include <ws2tcpip.h>
#include <vector>
#include <memory>
#include "IncomingHandlerExternal.h"
#include <vld.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


MainWindow::MainWindow()
{
	LoadDLL();
	master = new XCPMaster(TransportLayer::ETHERNET);
	master->SetSeedAndKeyFunctionPointers(GetAvailablePrivileges, ComputeKeyFromSeed);
	SetupConnection(s);
	Handler = new IncomingHandlerExternal(this);
	master->SetExternalMessageHandler(Handler);
}

MainWindow::~MainWindow()
{
	delete master;
	Cleanup(s);
}

bool MainWindow::OnInit()
{
	TestXCP();
	mainFrame = new LineChartFrame("Conti XCP Measurement Test",*this);
	mainFrame->Show(true);
	return true;
}

void MainWindow::AddPoint(double point)
{
	asd.push_back(point);
}

int MainWindow::LoadDLL()
{
	HINSTANCE hGetProcIDDLL = LoadLibrary(_T("SeedNKeyXcp.dll"));
	if (!hGetProcIDDLL) {
		std::cout << "could not load the dynamic library" << std::endl;
		return -1;
	}

	GetAvailablePrivileges = (XCP_GetAvailablePrivilegesPtr_t)GetProcAddress(hGetProcIDDLL, "XCP_GetAvailablePrivileges");
	if (!GetAvailablePrivileges) {
		std::cout << "could not locate the function" << std::endl;
		return -1;
	}

	ComputeKeyFromSeed = (XCP_ComputeKeyFromSeedPtr_t)GetProcAddress(hGetProcIDDLL, "XCP_ComputeKeyFromSeed");
	if (!GetAvailablePrivileges) {
		std::cout << "could not locate the function" << std::endl;
		return -1;
	}

	return 0;
}

void MainWindow::Cleanup(SOCKET s)
{
	closesocket(s);
	WSACleanup();
}

void MainWindow::Send(SOCKET s, XCPMsgPtr message)
{
	std::vector<uint8_t> bytes;
	message->Serialize(bytes);
	send(s, (const char*)bytes.begin()._Ptr, bytes.size(), 0);
	bytes.clear();
	bytes.resize(MaxRecvsize);
	master->AddSentMessage(message.get());

	int recv_size = recv(s, (char*)&bytes[0], MaxRecvsize, 0);
	bytes.resize(recv_size);
	for (int i = 0; i < recv_size; i++)
	{
		std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
	}
	std::cout << "\n";
	XCPMsgPtr asd = master->DeserializeMessage(bytes);
	//bytes.clear();
}


int MainWindow::SetupConnection(SOCKET& s)
{
	WSADATA wsa;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(5555);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");
	return 0;
}

void MainWindow::TestXCP()
{
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
	
		XCPMsgPtr AllocDaq = master->CreateAllocDaqMessage(1);
		Send(s, std::move(AllocDaq));
	
		XCPMsgPtr AllocOdt = master->CreateAllocOdtMessage(0, 1);
		Send(s, std::move(AllocOdt));
	
		XCPMsgPtr AllocOdtEntry = master->CreateAllocOdtEntryMessage(0, 0, 2);
		Send(s, std::move(AllocOdtEntry));
	
		XCPMsgPtr SetDaqPtr1 = master->CreateSetDaqPtrMessage(0, 0, 0);
		Send(s, std::move(SetDaqPtr1));
	
		XCPMsgPtr WriteDaq1 = master->CreateWriteDaqMessage(0xFF, 1, 0, 0x21A1BD); //sbyte triangle signal
		Send(s, std::move(WriteDaq1));

		/*XCPMsgPtr SetDaqPtr2 = master->CreateSetDaqPtrMessage(0, 0, 1);
		Send(s, std::move(SetDaqPtr2));

		XCPMsgPtr WriteDaq2 = master->CreateWriteDaqMessage(0xFF, 1, 0, 0x21A1BD); //sbyte triangle signal
		Send(s, std::move(WriteDaq2));*/
	
		using ModeFieldBits = SetDaqListModePacket::ModeFieldBits;
		XCPMsgPtr SetDaqListMode = master->CreateSetDaqListModeMessage(ModeFieldBits::TIMESTAMP, 0, 1, 1, 1); //DAQ direction; Timestamp on; do not use ctr field; Disabled alternating display; Transmit DTO WITH identification field;
		Send(s, std::move(SetDaqListMode));
	
		XCPMsgPtr StartStopDaqList = master->CreateStartStopDaqListMessage(StartStopDaqListPacket::Mode::SELECT, 0);
		Send(s, std::move(StartStopDaqList));
	
		XCPMsgPtr StartStopSynch = master->CreateStartStopSynchMessage(StartStopSynchPacket::Mode::START_SELECTED);
		Send(s, std::move(StartStopSynch));
	
		std::vector<uint8_t> bytes;
		int i = 0;
		while (i < 300)
		{
			bytes.resize(MaxRecvsize);
			int recv_size = recv(s, (char*)&bytes[0], MaxRecvsize, 0);
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
	
		XCPMsgPtr Stop = master->CreateStartStopSynchMessage(StartStopSynchPacket::Mode::STOP_ALL);
		Send(s, std::move(Stop));
	
		XCPMsgPtr disconnect_message = master->CreateDisconnectMessage();
		Send(s, std::move(disconnect_message));
}


//wxIMPLEMENT_APP(MainWindow);
wxIMPLEMENT_APP_CONSOLE(MainWindow);
