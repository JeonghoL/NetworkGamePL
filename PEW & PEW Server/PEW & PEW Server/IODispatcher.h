#pragma once

#include <concurrent_queue.h>
#include <vector>

using namespace concurrency;

class PacketHeader;

class IODispatcher {
public:
	void PushRecvPacket(int id, PacketHeader* packet);
	void PushSendPacket(int id, PacketHeader* packet);
	bool PopRecvPacket(std::pair<int, PacketHeader*>& out);
	bool PopSendPacket(std::pair<int, PacketHeader*>& out);

private:
	concurrent_queue<std::pair<int, PacketHeader*>> m_recvQueue;
	concurrent_queue<std::pair<int, PacketHeader*>> m_sendQueue;
};

