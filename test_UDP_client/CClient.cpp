#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "CClient.h"

#include <iostream>

#pragma comment(lib, "ws2_32.lib")

/// CLIENT UDP ///

Client_UDP::Client_UDP()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) //Initialize ws2_32.dll through a process
	{
		std::cout << "CLIENT_UDP: Initialize WSA failed" << std::endl;
		return;
	}

	m_sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	std::cout << "CLIENT_UDP: Client was created" << std::endl;

	m_TargetServer.sin_addr.S_un.S_addr = inet_addr(address);
	m_TargetServer.sin_family = AF_INET;
	m_TargetServer.sin_port = htons(port_udp);

	if (m_sockClient == -1)
	{
		std::cout << "CLIENT_UDP: Create socket failed!" << std::endl;
		WSACleanup();
	}
	else
	{
		//Send information to establish a connection with the server(must be added)
		sendto(m_sockClient, "hello server", strlen("hello server"), 0, (sockaddr*)&m_TargetServer, sizeof(m_TargetServer));
		std::cout << "CLIENT_UDP: Hello message was sending!" << std::endl;
	}
}

void Client_UDP::SendMsg(const char sendBuf[])
{
	sendto(m_sockClient, sendBuf, strlen(sendBuf), 0, (sockaddr*)&m_TargetServer, sizeof(m_TargetServer));
}

void Client_UDP::RecMsg(char* data, int leng)
{
	//std::cout << "Begin rec...(client)" << std::endl;
	int len = recvfrom(m_sockClient, data, leng, 0, 0, 0);
	if (len > 0)
	{
		data[len] = '\0';
	}

}
Client_UDP::~Client_UDP()
{
	closesocket(m_sockClient);
	WSACleanup();
}


/// CLIENT TCP ///
Client_TCP::Client_TCP()
{
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(address);
	addr.sin_port = htons(port_tcp);
	addr.sin_family = AF_INET;

	server_TCP = socket(AF_INET, SOCK_STREAM, NULL);
	if (server_TCP < 0)
	{
		std::cout << "CLIENT_TCP: Socket error" << std::endl;
		exit(0);
	}
	std::cout << "CLIENT_TCP: Socket for client created" << std::endl;

	if (connect(server_TCP, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "CLIENT_TCP: failed connect to server.\n";
	}
	std::cout << "CLIENT_TCP: TCP_server succes connected!\n";

	server_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_UDP < 0)
	{
		std::cout << "CLIENT_TCP: Socket error" << std::endl;
		exit(0);
	}
}

int Client_TCP::sendData(const char * buffer, int len)
{
	return send(server_TCP, buffer, len, NULL);
}

int Client_TCP::recvData(char *buffer, int len)
{
	return recv(server_TCP, buffer, 1024, NULL);
}

Client_TCP::~Client_TCP()
{
	closesocket(server_TCP);
	std::cout << "Server is closed" << std::endl;
}