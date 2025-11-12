#include "PacketFactory.h"
#include "protocol.hpp"

std::vector<char> PacketFactory::SCLoginPacket()
{
	SC_LOGIN_PACKET login;
	login.size = sizeof(login);
	login.type = SC_LOGIN;

	return Serialize(login);
}