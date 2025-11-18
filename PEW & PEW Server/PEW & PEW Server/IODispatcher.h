#pragma once

#include <concurrent_queue.h>
#include <vector>

using namespace concurrency;

class IODispatcher {
public:
	static IODispatcher& Get()
	{
		static IODispatcher instance;
		return instance;
	}

	void PushRecvPacket(size_t id, const std::vector<char>& packet);
	void PushSendPacket(size_t id, const std::vector<char>& packet);
	bool PopRecvPacket(std::pair<size_t, std::vector<char>>& out);
	bool PopSendPacket(std::pair<size_t, std::vector<char>>& out);

private:
	concurrent_queue<std::pair<size_t, std::vector<char>>> m_recvQueue;
	concurrent_queue<std::pair<size_t, std::vector<char>>> m_sendQueue;
};

