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
 
vector<char> PacketFactory::CSMovePacket(float angle, char direction, bool run)
{
	CS_MOVE_PACKET move;
	move.size = sizeof(move);
	move.type = CS_MOVE;
	move.angle = angle;
	move.direction = direction;
	move.isRun = run;

	return Serialize(move);
}
