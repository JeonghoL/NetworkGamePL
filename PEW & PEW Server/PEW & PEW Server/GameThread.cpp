#include "GameThread.h"

GameThread::GameThread() : m_running(false)
{
	// TODO : 패킷 핸들러 등록
}

void GameThread::Start()
{
	m_running = true;
	m_thread = std::thread(&GameThread::ThreadFunc, this);
}

void GameThread::Stop()
{
	m_running = false;

	if (m_thread.joinable())
		m_thread.join();
}

void GameThread::ThreadFunc()
{
	// TODO : 게임 로직 처리 루프
}

void GameThread::ProcessPacket(std::pair<int, PacketHeader*>& packetInfo)
{
	// TODO : 패킷 처리 로직
}
