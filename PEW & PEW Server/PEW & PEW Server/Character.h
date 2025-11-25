#pragma once

#include <optional>
#include <vector>
#include "GameObject.h"

class Character : public GameObject {
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

	virtual void Update(const float dT) override;
	virtual bool IsAlive() const override;

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

private:
	void Move(const float dT);

	char _direction;
	bool _isRun;

	float _angle;
	bool _angleChange;

	int _hp;
	bool _isAlive;
	bool _isDeadProcessed;

	int _texture;

	std::optional<AttackSequence> _attackSeq;
};

