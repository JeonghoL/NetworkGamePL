#pragma once

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

class ServerCore {
public:
	static ServerCore& GetInstance()
	{
		static ServerCore instance;
		return instance;
	}

	void Start()
	{
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			return;

		m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_listenSocket == INVALID_SOCKET)
			return;

		sockaddr_in serverAddr;
		ZeroMemory(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddr.sin_port = htons(9000);

		if (bind(m_listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
			return;

		if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
			return;

		std::cout << "ServerCore Start" << std::endl;
		AcceptLoop();
	}

private:
	void AcceptLoop()
	{
		while (true)
		{
			if (SOCKET clientSocket = accept(m_listenSocket, nullptr, nullptr)) 
			{
				// TODO : 빈 IOThread를 찾아서 연결
				//        빈 IOThread가 없으면 접속 거부
			}
		}
	}



private:
	SOCKET m_listenSocket;
};

