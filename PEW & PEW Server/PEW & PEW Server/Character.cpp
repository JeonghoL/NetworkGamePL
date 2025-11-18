#include "Character.h"
#include "protocol.hpp"
#include "GameObject.h"

#include <algorithm>

Character::Character(int i)
	: GameObject(i, ObjectType::Character), _direction(-1), _isRun(false), _angle(0.0f)
{
}

void Character::Update(const float dT)
{
	Move(dT);
}

inline bool Character::IsAlive() const
{
	return false;
}

void Character::Move(const float dT)
{
	if (_direction < 0 or _direction >= 8) return;

	float baseMove = (_isRun ? 3.0f : 1.5f);
	float moveDistance = baseMove * dT;

	static constexpr vec3 dirTable[8] = {
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f,  1.0f },
		{ -1.0f, 0.0f, 0.0f },
		{  1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, -1.0f },
		{ 1.0f, 0.0f,  -1.0f },
		{ -1.0f, 0.0f,  1.0f },
		{ 1.0f, 0.0f,  1.0f }
	};

	vec3 moveVec = dirTable[_direction];

	if (_direction >= MoveDirection::UPLEFT) {
		float normal = sqrt(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
		if (normal > 0.0f) {
			moveVec = moveVec / normal;
		}
	}

	position += moveVec * moveDistance;
	position.x = std::clamp(position.x, -14.5f, 14.5f);
	position.z = std::clamp(position.z, -14.6f, 14.4f);

	version++;
}
