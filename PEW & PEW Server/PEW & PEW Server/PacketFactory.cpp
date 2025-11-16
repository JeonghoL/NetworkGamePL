#include "PacketFactory.h"
#include "protocol.hpp"
#include "GameObject.h"

std::vector<char> PacketFactory::SCLoginPacket()
{
	SC_LOGIN_PACKET login;
	login.size = sizeof(login);
	login.type = SC_LOGIN;

	return Serialize(login);
}

std::vector<char> PacketFactory::SCMovePacket(GameObject* obj)
{
	SC_MOVE_PACKET move;

	switch (obj->type) {
	case ObjectType::Character:
	{
		// TODO : Character 이동
		break;
	}
	case ObjectType::Projectile:
	{
		// TODO : Projectile 이동
		break;
	}
	}

	return Serialize(move);
}
