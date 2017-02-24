#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "5555"

int main()
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	char in_buffer[2000];

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5555);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	//Listen to incoming connections
	listen(s, 1);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}

	puts("Connection accepted");

	int in_length = recv(new_socket, (char*)&in_buffer, 2000, 0);
	
	for (int i = 0; i < in_length; i++)
	{
		std::cout << std::hex <<int(in_buffer[i]&0xff) << " ";
	}
	std::cout << "\n";


	//Reply to client
	unsigned char message[] = { 0x08,0x00,0x00,0x00,0xff,0x1d,0xc0,0xff,0xdc,0x05,0x01,0x01 };
	send(new_socket, (char*)message, sizeof(message), 0);
	in_length = recv(new_socket, (char*)&in_buffer, 2000, 0);

	for (int i = 0; i < in_length; i++)
	{
		std::cout << std::hex << int(in_buffer[i] & 0xff) << " ";
	}
	std::cout << "\n";

	getchar();

	closesocket(s);
	WSACleanup();

	return 0;
}