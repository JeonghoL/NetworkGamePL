#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>

enum PacketType;
class PacketHeader;

class GameThread {
	using PacketHandler = std::function<void(std::pair<int, PacketHeader*>&)>;

public:
	GameThread();
	~GameThread() { Stop(); }

	void Start();
	void Stop();

private:
	void ThreadFunc();
	void ProcessPacket(std::pair<int, PacketHeader*>& packetInfo);

	std::thread m_thread;
	std::unordered_map<PacketType, PacketHandler> m_handlerTable;

	std::atomic<bool> m_running;
};

