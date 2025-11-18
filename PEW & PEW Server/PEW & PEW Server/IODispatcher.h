#pragma once

#include <concurrent_queue.h>
#include <array>
#include <vector>

using namespace concurrency;

class IODispatcher {
public:
	static IODispatcher& Get()
	{
		static IODispatcher instance;
		return instance;
	}

	void Broadcast(const std::vector<char>& packet);

	concurrent_queue<std::pair<int, std::vector<char>>> recvQueue;
	std::array<concurrent_queue<std::vector<char>>, 3> sendQueues;
};