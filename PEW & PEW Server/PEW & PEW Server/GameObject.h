#pragma once

#include <math.h>

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

enum class ObjectType {
	Character,
	Projectile
};

class GameObject {
public:
	GameObject(int i, ObjectType t) : id(i), type(t)
	{
		position = { 0.0f, 0.0f, 0.0f };
		version = 0;
	}
	virtual ~GameObject() = default;

	virtual void Update(const float dT) = 0;
	virtual bool IsAlive() const = 0;

	int id;
	ObjectType type;
	vec3 position;
	size_t version;
};

