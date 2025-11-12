#include "GameThread.h"
#include "IODispatcher.h"
#include "protocol.hpp"

GameThread::GameThread() : m_running(false)
{
	// TODO : 패킷 핸들러 등록

	m_handlerTable[CS_LOGIN] =
		[this](std::pair<int, std::vector<char>>& packetInfo)
		{
			this->TestHandler(packetInfo);
		};
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

void GameThread::ProcessPacket(std::pair<int, std::vector<char>>& packetInfo)
{
	const char type = packetInfo.second[1];
	auto it = m_handlerTable.find(type);
	if (it != m_handlerTable.end())
		it->second(packetInfo);

	else
		std::cout << "Unknown Packet Type: " << type << std::endl;
}

void GameThread::TestHandler(std::pair<int, std::vector<char>>& packetInfo)
{
	// 패킷 핸들러에서 수행해야할 것
	//
	// 1. 직렬화된 패킷을 자신이 처리해야하는 구체 패킷 class로 casting
	// 2. PacketType에 맞는 처리
	std::cout << "TestHandler" << std::endl;
}
