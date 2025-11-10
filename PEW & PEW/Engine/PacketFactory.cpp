#include "pch.h"
#include "PacketFactory.h"

vector<char> PacketFactory::CSLoginPacket(int characterType)
{
	CS_LOGIN_PACKET login;
	login.size = sizeof(login);
	login.type = CS_LOGIN;
	login.textureId = characterType;

	return Serialize(login);
}
 