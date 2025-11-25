#pragma once

#include <vector>
#include "Projectile.h"

class CollisionManager {
public:
	CollisionManager() = delete;

	std::vector<std::pair<Projectile*, Character*>>
		GetCollisionList(const std::vector<Projectile*> projectiles, const std::vector<Character*> characters);

	static bool SphereAABBCollision(const vec3& sphereCenter, float sphereRadius, const vec3& minAABB, const vec3& maxAABB);
	static bool AABBCollision(const vec3& minA, const vec3& maxA, const vec3& minB, const vec3& maxB);
};

