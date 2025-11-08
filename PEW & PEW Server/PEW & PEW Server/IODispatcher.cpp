#include "IODispatcher.h"

void IODispatcher::PushRecvPacket(int id, PacketHeader* packet)
{
	m_recvQueue.push(std::make_pair(id, packet));
}

void IODispatcher::PushSendPacket(int id, PacketHeader* packet)
{
	m_sendQueue.push(std::make_pair(id, packet));
}

bool IODispatcher::PopRecvPacket(std::pair<int, PacketHeader*>& out)
{
	return m_recvQueue.try_pop(out);
}

bool IODispatcher::PopSendPacket(std::pair<int, PacketHeader*>& out)
{
	return m_sendQueue.try_pop(out);
}
