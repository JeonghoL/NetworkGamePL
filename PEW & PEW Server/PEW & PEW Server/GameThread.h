#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>

class GameThread {
	using PacketHandler = std::function<void(std::pair<int, std::vector<char>>&)>;

public:
	GameThread();
	~GameThread() { Stop(); }

	void Start();
	void Stop();

private:
	void ThreadFunc();
	void ProcessPacket(std::pair<int, std::vector<char>>& packetInfo);

	void TestHandler(std::pair<int, std::vector<char>>& packetInfo);

	std::thread m_thread;
	std::unordered_map<char, PacketHandler> m_handlerTable;

	std::atomic<bool> m_running;
};

