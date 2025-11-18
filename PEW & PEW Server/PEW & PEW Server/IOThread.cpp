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
    std::vector<char> recvBuffer;
    recvBuffer.reserve(8192);

    auto& dispatcher = IODispatcher::Get();

    constexpr int TEMP_BUFFER = 4096;
    std::vector<char> temp(TEMP_BUFFER);

    while (m_running)
    {
        // 1) blocking recv
        int ret = recv(m_clientSocket, temp.data(), TEMP_BUFFER, 0);

        if (ret > 0)
        {
            // 받은 만큼 붙이기
            recvBuffer.insert(recvBuffer.end(), temp.begin(), temp.begin() + ret);

            // 2) 패킷 단위 추출
            while (true)
            {
                if (recvBuffer.size() < 1)
                    break; // 최소 size도 없음

                unsigned char packetSize = recvBuffer[0];

                if (recvBuffer.size() < packetSize)
                    break; // 아직 패킷이 다 안 들어옴

                // 정상 패킷 하나 추출
                std::vector<char> onePacket(
                    recvBuffer.begin(),
                    recvBuffer.begin() + packetSize
                );

                dispatcher.recvQueue.push({ m_id, std::move(onePacket) });

                // 버퍼에서 소비한 만큼 제거
                recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + packetSize);
            }
        }
        else if (ret == 0)
        {
            // 정상 종료
            Stop();
            break;
        }
        else
        {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK && err != WSAEINTR)
            {
                Stop();
                break;
            }
        }

        // 3) Send queue 처리 (non-blocking)
        std::vector<char> pktToSend;
        while (dispatcher.sendQueues[m_id].try_pop(pktToSend))
        {
            send(m_clientSocket, pktToSend.data(), (int)pktToSend.size(), 0);
        }
    }
}
