#include "IODispatcher.h"

void IODispatcher::PushRecvPacket(size_t id, const std::vector<char>& packet)
{
	m_recvQueue.push(std::make_pair(id, packet));
}

void IODispatcher::PushSendPacket(size_t id, const std::vector<char>& packet)
{
	m_sendQueue.push(std::make_pair(id, packet));
}

bool IODispatcher::PopRecvPacket(std::pair<size_t, std::vector<char>>& out)
{
	return m_recvQueue.try_pop(out);
}

bool IODispatcher::PopSendPacket(std::pair<size_t, std::vector<char>>& out)
{
	return m_sendQueue.try_pop(out);
}

