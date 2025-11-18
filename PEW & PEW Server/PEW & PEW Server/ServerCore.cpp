#include "ServerCore.h"

void ServerCore::Start()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
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

void ServerCore::AcceptLoop()
{
	while (true)
	{
		if (SOCKET clientSocket = accept(m_listenSocket, nullptr, nullptr))
		{
			if (IOThread* ioThread = GetIdleIOThread())
			{
				ioThread->Start(clientSocket, nextId);
				nextId = (nextId + 1) % 3;
			}

			else
				closesocket(clientSocket);
		}
	}
}

IOThread* ServerCore::GetIdleIOThread()
{
	for (IOThread& ioThread : m_ioThreads)
	{
		if (!ioThread.IsRunning())
			return &ioThread;
	}

	return nullptr;
}
														