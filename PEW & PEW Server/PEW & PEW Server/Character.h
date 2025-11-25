#pragma once

#include <optional>
#include <vector>

struct vec3 {
	float x;
	float y;
	float z;

	vec3 operator+(const vec3& other) const
	{
		return vec3{ x + other.x, y + other.y, z + other.z };
	}

	vec3 operator-(const vec3& other) const
	{
		return vec3{ x - other.x, y - other.y, z - other.z };
	}

	vec3 operator*(float other) const
	{
		return vec3{ x * other, y * other, z * other };
	}

	vec3 operator/(float other) const
	{
		return vec3{ x / other, y / other, z / other };
	}

	vec3& operator+=(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	float DistanceSq(const vec3& other) const
	{
		vec3 deltaVec = *this - other;
		return powf(deltaVec.x, 2) + powf(deltaVec.y, 2) + powf(deltaVec.z, 2);
	}

	vec3 Cross(const vec3& other) const
	{
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}

	vec3 Normalize() const
	{
		float len = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
		if (len == 0) return { 0, 0, 0 };
		return *this / len;
	}
};

class Character {
	struct AttackSequence {
		vec3 direction;
		std::vector<float> attackTimes;
	};

	static constexpr int NUMBER_OF_ATTACK{ 3 };
	static constexpr float INTERVAL_OF_ATTACK{ 0.15f };

	static constexpr float SIZE_X{ 0.5f };
	static constexpr float SIZE_Y{ 0.95f };
	static constexpr float SIZE_Z{ 0.4f };

	static constexpr int MAX_HP{ 100 };

	static constexpr vec3 DEFAULT_POS[2] = {
		{ 13.6f, 0.0f, 0.0f },
		{ -13.6f, 0.0f, 0.0f }
	};

public:
	Character(int i);
	virtual ~Character() = default;

	void Update(const float dT);
	bool IsAlive() const;

	void TakeDamage(int damage);
	std::optional<vec3> GetNextProjectile(float nowTime);
	std::pair<vec3, vec3> GetCollisionBox() const;
	void SetInput(float angle, char direction, bool isRun);
	void SetAttackSequence(float nowTime, const vec3& dir);
	void Death();
	void Revive();

	float GetAngle() const { return _angle; }
	bool IsMove() const { return _direction >= 0 and _direction < 8; }
	bool IsRun() const { return _isRun; }
	int GetId() const { return _id; }
	int GetHp() const { return _hp; }
	vec3 GetPosition() const { return _pos; }
	int GetTexture() const { return _texture; }

private:
	void Move(const float dT);

	int _id;

	char _direction;
	bool _isRun;

	vec3 _pos;
	float _angle;
	bool _angleChange;

	int _hp;
	bool _isAlive;
	bool _isDeadProcessed;

	long long _version;

	int _texture;

	std::optional<AttackSequence> _attackSeq;
};

