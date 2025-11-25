#include "GameThread.h"
#include "IODispatcher.h"
#include "protocol.h"
#include "PacketFactory.h"
#include "Character.h"

#include <iostream>

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
		DispatchPackets();

		auto now = steady_clock::now();
		accumlator += duration<float>(now - prev).count();
		prev = now;

		while (accumlator >= TickTime)
		{
			// 1. 상태 Update
			//_objMng.Update(TickTime);

			// 2. 충돌처리
			// 3. etc...

			accumlator -= TickTime;
		}
	}
}

void GameThread::DispatchPackets()
{
	auto& dispatcher = IODispatcher::Get();

	std::pair<int, std::vector<char>> info;
	while (dispatcher.recvQueue.try_pop(info))
	{
		char type = info.second[1];
		auto it = m_handlerTable.find(type);

		if (it != m_handlerTable.end())
			it->second(info);
	}
}

void GameThread::LoginHandler(std::pair<int, std::vector<char>>& packetInfo)
{
	int sessId = packetInfo.first;
	const std::vector<char>& packet = packetInfo.second;

	auto login = PacketFactory::Deserialize<CS_LOGIN_PACKET>(packet);
	
	/*auto* character = _objMng.CreatObject<Character>(sessId);

	auto& dispatcher = IODispatcher::Get();
	dispatcher.sendQueues[sessId].push(PacketFactory::SCLoginPacket());

	dispatcher.Broadcast(PacketFactory::SCAddPacket(character));

	for (auto& [id, obj] : _objMng.GetObjects())
	{
		if (sessId == id) continue;
		dispatcher.sendQueues[sessId].push(PacketFactory::SCAddPacket(obj.get()));
	}*/
}

void GameThread::MoveHandler(std::pair<int, std::vector<char>>& packetInfo)
{
}
