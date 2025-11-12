#include "PacketFactory.h"

std::vector<char> PacketFactory::SCLoginPacket()
{
	SC_LOGIN_PACKET login;
	return Serialize(login);
}
