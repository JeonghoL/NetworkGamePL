#include "IODispatcher.h"

void IODispatcher::Broadcast(const std::vector<char>& packet)
{
	for (auto& sendQueue : sendQueues)
	{
		sendQueue.push(packet);
	}
}
