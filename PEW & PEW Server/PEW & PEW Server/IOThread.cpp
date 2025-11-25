#include "IOThread.h"
#include "IODispatcher.h"
#include "PacketFactory.h"

#include <vector>

IOThread::IOThread()
	: m_id(-1), m_clientSocket(INVALID_SOCKET), m_running(false)
{
}

void IOThread::Start(SOCKET socket, int id)
{
	m_id = id;
	m_clientSocket = socket;

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

	//if (m_thread.joinable())
	//	m_thread.join();
}

void IOThread::Thread_Func()
{
    auto& dispatcher = IODispatcher::Get();

    while (m_running)
    {
		unsigned char sizeByte{ 0 };
		int ret = recv(m_clientSocket, reinterpret_cast<char*>(&sizeByte), 1, 0);

		if (ret == 0) break;
		else if (ret < 0)
		{
			int err = WSAGetLastError();
			if (err == WSAEINTR) continue;
			else break;
		}

		unsigned char packetSize = sizeByte;
		if (packetSize < 2) break;

		std::vector<char> packet(packetSize);
		packet[0] = sizeByte;

		int toRead = packetSize - 1;
		int offset = 1;

		int r = recv(m_clientSocket, packet.data() + offset, toRead, MSG_WAITALL);
		if (r <= 0) break;
		else dispatcher.recvQueue.push(std::make_pair(m_id, packet));

		std::vector<char> sendPkt;
		while (dispatcher.sendQueues[m_id].try_pop(sendPkt))
		{
			int total = sendPkt.size();
			int sent = 0;

			while (sent < total)
			{
				int s = send(m_clientSocket, sendPkt.data() + sent, total - sent, 0);
				if (s <= 0) break;
				else sent += s;
			}
		}
    }
}
