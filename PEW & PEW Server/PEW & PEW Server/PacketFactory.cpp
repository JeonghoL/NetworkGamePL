#include "PacketFactory.h"
#include "protocol.hpp"
#include "GameObject.h"
#include "Character.h"

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
		Character* character = static_cast<Character*>(obj);
		move.size = sizeof(move);
		move.type = SC_MOVE;
		move.id = obj->id;
		move.angle = character->GetAngle();

		vec3 charPos = character->position;

		move.x = charPos.x;
		move.y = charPos.y;
		move.z = charPos.z;

		move.isMove = character->IsMove();
		move.isRun = character->IsRun();

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

std::vector<char> PacketFactory::SCAddPacket(GameObject* obj)
{
	SC_ADD_PACKET add;

	switch (obj->type) {
	case ObjectType::Character:
	{
		Character* character = static_cast<Character*>(obj);
		add.size = sizeof(add);
		add.type = SC_ADD;
		add.id = character->id;

		vec3 charPos = character->position;

		add.x = charPos.x;
		add.y = charPos.y;
		add.z = charPos.z;

		break;
	}
	case ObjectType::Projectile:
	{
		// TODO : Projectile 추가
		break;
	}
	}

	return Serialize(add);
}

std::vector<char> PacketFactory::SCRemovePacket(GameObject* obj)
{
	SC_REMOVE_PACKET remove;

	switch (obj->type) {
	case ObjectType::Character:
	{
		Character* character = static_cast<Character*>(obj);
		remove.size = sizeof(remove);
		remove.type = SC_REMOVE;
		remove.id = character->id;

		break;
	}
	case ObjectType::Projectile:
	{
		// TODO : Projectile 삭제
		break;
	}
	}

	return Serialize(remove);
}
