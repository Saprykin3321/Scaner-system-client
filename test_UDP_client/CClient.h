#pragma once

#include <winsock2.h>

//const char		address[15] = "192.168.1.3"; // LattePanda
const char		address[15] = "127.0.0.1"; // PC


class Client_UDP
{
	SOCKET					m_sockClient;
	sockaddr_in				m_TargetServer;
	//unsigned short			port_udp = 1001; // LattePanda
	unsigned short			port_udp = 8090; // PC

public:
	Client_UDP();
	void RecMsg(char* data, int leng);
	void SendMsg(const char sendBuf[]);
	~Client_UDP();
};

class Client_TCP
{
	WSADATA				wsaData;
	SOCKET				server_TCP, server_UDP;
	SOCKADDR_IN			addr;
	int					sizeofaddr = sizeof(addr);
	char*				buff = nullptr;
	//unsigned short		port_tcp = 1002; // LattePanda
	unsigned short		port_tcp = 8091; // PC

public:
	Client_TCP();
	int sendData(const char* buffer, int len);
	int recvData(char* buffer, int len);
	~Client_TCP();
};