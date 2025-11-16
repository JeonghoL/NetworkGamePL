#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>

#include "ObjectManager.h"

class GameThread {
	using PacketHandler = std::function<void(std::pair<int, std::vector<char>>&)>;
	static constexpr float TickTime{ 1.0f / 60.0f };

public:
	GameThread();
	~GameThread() { Stop(); }

	void Start();
	void Stop();

private:
	void ThreadFunc();
	void ProcessPacket(std::pair<int, std::vector<char>>& packetInfo);

	void LoginHandler(std::pair<int, std::vector<char>>& packetInfo);
	void MoveHandler(std::pair<int, std::vector<char>>& packetInfo);

	ObjectManager _objMng;

	std::thread m_thread;
	std::unordered_map<char, PacketHandler> m_handlerTable;

	std::atomic<bool> m_running;
};

