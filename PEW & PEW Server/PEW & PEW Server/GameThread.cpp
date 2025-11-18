#include "GameThread.h"
#include "IODispatcher.h"
#include "protocol.hpp"
#include "PacketFactory.h"
#include "Character.h"

GameThread::GameThread() : m_running(false)
{
	// TODO : 패킷 핸들러 등록

	m_handlerTable[CS_LOGIN] =
		[this](std::pair<int, std::vector<char>>& packetInfo)
		{
			this->LoginHandler(packetInfo);
		};

	m_handlerTable[CS_MOVE] =
		[this](std::pair<int, std::vector<char>>& packetInfo)
		{
			this->MoveHandler(packetInfo);
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
	using namespace std::chrono;

	float accumlator{ 0.0f };
	auto prev = steady_clock::now();
	while (m_running)
	{
		auto now = steady_clock::now();
		accumlator += duration<float>(now - prev).count();
		prev = now;

		while (accumlator >= TickTime)
		{
			// 1. 상태 Update
			_objMng.Update(TickTime);

			// 2. 충돌처리
			// 3. etc...


			accumlator -= TickTime;
		}
	}
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

void GameThread::LoginHandler(std::pair<int, std::vector<char>>& packetInfo)
{
	int sessId = packetInfo.first;
	const std::vector<char>& packet = packetInfo.second;

	auto login = PacketFactory::Deserialize<CS_LOGIN_PACKET>(packet);
	
	_objMng.CreatObject<Character>(sessId);
	IODispatcher::Get().PushSendPacket(sessId, PacketFactory::SCLoginPacket());


}

void GameThread::MoveHandler(std::pair<int, std::vector<char>>& packetInfo)
{
	std::cout << "MoveHandler" << std::endl;
}
