#include "IOThread.h"
#include "IODispatcher.h"
#include "PacketFactory.h"

#include <vector>

IOThread::IOThread()
	: m_clientSocket(INVALID_SOCKET), m_running(false)
{
}

void IOThread::Start(SOCKET socket)
{
	m_running = true;
	m_thread = std::thread(&IOThread::Thread_Func, this);
}

void IOThread::Stop()
{
	m_running = false;

	if (m_clientSocket != INVALID_SOCKET)
	{
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	}

	if (m_thread.joinable())
		m_thread.join();
}

void IOThread::Thread_Func()
{
	// TODO : 
	//  1. recv() 
	//  2. IODispatcher로 수신패킷 전달 
	//  3. 송신해야할 패킷 있으면 send() 호출

	//std::vector<char> buffer;
	//recv(m_clientSocket, buffer.data(), /*Temp*/, 0);

	//IODispatcher::Get().PushRecvPacket(m_clientSocket, buffer);
}
