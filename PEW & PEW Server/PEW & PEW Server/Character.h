#pragma once

#include "GameObject.h"

class Character : public GameObject {
public:
	Character(int i);
	virtual ~Character() = default;

	virtual void Update(const float dT) override;
	virtual bool IsAlive() const override;

	float GetAngle() const { return _angle; }
	bool IsMove() const { return _direction >= 0 and _direction < 8; }
	bool IsRun() const { return _isRun; }

private:
	void Move(const float dT);

	char _direction;
	bool _isRun;

	float _angle;
};

