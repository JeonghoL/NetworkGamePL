#pragma once

#include <iostream>
#include <array>
#include <WinSock2.h>

#include "IOThread.h"
#include "GameThread.h"

#pragma comment(lib, "ws2_32.lib")


class ServerCore {
public:
	static ServerCore& Get()
	{
		static ServerCore instance;
		return instance;
	}

	void Start();

private:
	void AcceptLoop();
	IOThread* GetIdleIOThread();

private:
	std::array<IOThread, 2> m_ioThreads;
	GameThread m_gameThread;

	SOCKET m_listenSocket;
};

