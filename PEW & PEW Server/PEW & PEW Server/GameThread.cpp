#include "GameThread.h"

GameThread::GameThread() : m_running(false)
{
	// TODO : 패킷 핸들러 등록
	//
	// 패킷 핸들러에서 수행해야할 것
	//
	// 1. PacketHeader*를 자신이 처리해야하는 구체 패킷 class로 casting
	// 2. PacketType에 맞는 처리
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
	//
	// 1. 상태 Update
	// 2. 충돌처리
	// 3. etc...
}

void GameThread::ProcessPacket(std::pair<int, PacketHeader*>& packetInfo)
{
	// TODO : 패킷 처리 로직
	//
	// 1. PacketHeader* 에서 PacketType 확인
	// 2. PacketType에 맞는 Handler 함수로 매핑
}
