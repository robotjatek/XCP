#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef XCPLIB_EXPORTS
#define XCP_API    __declspec(dllexport)
#else
#define XCP_API    __declspec(dllimport)
#endif



#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <memory>

#include <vld.h>

#include <XCPMaster.h>
#include <ResponsePacket.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5555"


using XCPMsgPtr = std::unique_ptr<IXCPMessage>;
XCPMaster master(TransportLayer::ETHERNET);

int SetupConnection(SOCKET& s)
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


void Cleanup(SOCKET s)
{
	closesocket(s);
	WSACleanup();
}

void Send(SOCKET s, XCPMsgPtr message)
{
	std::vector<uint8_t> bytes;
	message->Serialize(bytes);
	send(s, (const char*)bytes.begin()._Ptr, bytes.size(), 0);
	bytes.clear();
	bytes.resize(2000);
	master.AddSentMessage(message.get());

	int recv_size = recv(s, (char*)&bytes[0], 2000, 0);
	bytes.resize(recv_size);
	for (int i = 0; i < recv_size; i++)
	{
		std::cout << std::hex << (int)(bytes[i] & 0xff) << " ";
	}
	std::cout << "\n";
	XCPMsgPtr asd = master.DeserializeMessage(bytes);
	bytes.clear();
}

int main()
{
	SOCKET s;
	if (SetupConnection(s))
	{
		return 1;
	}
	
	XCPMsgPtr connect_message = master.CreateConnectMessage(ConnectMode::NORMAL);
	XCPMsgPtr disconnect_message = master.CreateDisconnectMessage();
	XCPMsgPtr GetStatus = master.CreateGetStatusMessage();
	XCPMsgPtr Synch = master.CreateSynchMessage();
	XCPMsgPtr SetMTA = master.CreateSetMTAMessage(0x219020, 0);	
	//XCPMsgPtr SetMTA = master.CreateSetMTAMessage(0x0, 0);
	XCPMsgPtr Upload = master.CreateUploadMessage(10);
	XCPMsgPtr ShortUpload = master.CreateShortUploadMessage(10, 0x219020, 0);
	XCPMsgPtr FreeDaq = master.CreateFreeDaqMessage();
	XCPMsgPtr AllocDaq = master.CreateAllocDaqMessage(1);
	XCPMsgPtr AllocOdt = master.CreateAllocOdtMessage(0, 1);
	XCPMsgPtr AllocOdtEntry = master.CreateAllocOdtEntryMessage(0, 0, 10);

	Send(s, std::move(connect_message));
	Send(s, std::move(GetStatus));
	Send(s, std::move(Synch));
	Send(s, std::move(SetMTA));
	Send(s, std::move(Upload));
	Send(s, std::move(ShortUpload));
	Send(s, std::move(FreeDaq));
	Send(s, std::move(AllocDaq));
	Send(s, std::move(AllocOdt));
	Send(s, std::move(AllocOdtEntry));
	Send(s, std::move(disconnect_message));

	Cleanup(s);
	system("pause");
	return 0;
}