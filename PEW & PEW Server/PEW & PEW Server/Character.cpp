#include "Character.h"
#include "protocol.h"

#include <algorithm>

Character::Character(int i)
	: _id(i)
{
	_pos = DEFAULT_POS[i % 2];
	_angle = 0.0f;
	_angleChange = false;

	_hp = MAX_HP;
	_isAlive = true;
	_isDeadProcessed = false;

	_version = 0;
}

void Character::Update(const float dT)
{
	Move(dT);
}

bool Character::IsAlive() const
{
	return _isAlive;
}

void Character::TakeDamage(int damage)
{
	if (not _isAlive) {
		return;
	}

	_hp -= damage;
	if (_hp <= 0) {
		Death();
	}
}

std::optional<vec3> Character::GetNextProjectile(float nowTime)
{
	if (not _attackSeq.has_value()) {
		return std::nullopt;
	}

	auto& seq = _attackSeq.value();
	if (seq.attackTimes.empty()) {
		_attackSeq.reset();
		return std::nullopt;
	}

	if (nowTime >= seq.attackTimes.front()) {
		seq.attackTimes.erase(seq.attackTimes.begin());

		if (seq.attackTimes.empty()) {
			_attackSeq.reset();
		}

		return seq.direction;
	}

	return std::nullopt;
}

std::pair<vec3, vec3> Character::GetCollisionBox() const
{
	const vec3 offsetMin{ SIZE_X / 2, 0, SIZE_Z / 2 };
	const vec3 offsetMax{ SIZE_X / 2, SIZE_Y, SIZE_Z / 2 };

	return { _pos - offsetMin, _pos + offsetMax };
}

void Character::SetInput(float angle, char direction, bool isRun)
{
	if (_angle != angle) {
		_angle = angle;
		_angleChange = true;
	}

	_direction = direction;
	_isRun = isRun;
}

void Character::SetAttackSequence(float nowTime, const vec3& dir)
{
	float attackOffset =
		_isRun ? 0.43f :
		IsMove() ? 0.56f :
		0.45f;

	if (_attackSeq.has_value()) {
		return;
	}

	std::vector<float> attackTimes(NUMBER_OF_ATTACK);
	std::generate_n(attackTimes.begin(), NUMBER_OF_ATTACK,
		[n = 0, &nowTime, &attackOffset, this]() mutable
		{
			return nowTime + attackOffset + (n++) * INTERVAL_OF_ATTACK;
		});

	_attackSeq = AttackSequence{ dir, attackTimes };
}

void Character::Death()
{
	_hp = 0;
	_isAlive = false;

	_direction = -1;
	_isRun = false;
	_angleChange = false;
}

void Character::Revive()
{
	_pos = DEFAULT_POS[_id % 2];
	_angle = 0.0f;
	_angleChange = false;

	_hp = MAX_HP;
	_isAlive = true;
	_isDeadProcessed = false;

	_direction = -1;
	_isRun = false;
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

	_pos += moveVec * moveDistance;
	_pos.x = std::clamp(_pos.x, -14.5f, 14.5f);
	_pos.z = std::clamp(_pos.z, -14.6f, 14.4f);

	_version++;
}

