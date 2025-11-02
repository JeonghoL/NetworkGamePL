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

	void Start();

private:
	void AcceptLoop();



private:
	SOCKET m_listenSocket;
};

