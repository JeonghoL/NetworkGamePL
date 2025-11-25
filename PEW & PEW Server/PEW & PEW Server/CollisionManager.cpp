#include "CollisionManager.h"

std::vector<std::pair<Projectile*, Character*>> CollisionManager::GetCollisionList(const std::vector<Projectile*> projectiles, const std::vector<Character*> characters)
{
	std::vector<std::pair<Projectile*, Character*>> out;

	for (auto& projectile : projectiles)
	{
		for (auto& character : characters)
		{
			if (!character->IsAlive()) continue;
			if (projectile->GetOwnerId() == character->GetId()) continue;
			if (projectile->CheckCollision(*character))
			{
				out.emplace_back(projectile, character);
				break;
			}
		}
	}

	return out;
}

bool CollisionManager::SphereAABBCollision(const vec3& sphereCenter, float sphereRadius, const vec3& minAABB, const vec3& maxAABB)
{
	vec3 closestPt = {
		std::max(minAABB.x, std::min(sphereCenter.x, maxAABB.x)),
		std::max(minAABB.y, std::min(sphereCenter.y, maxAABB.y)),
		std::max(minAABB.z, std::min(sphereCenter.z, maxAABB.z))
	};

	return sphereCenter.DistanceSq(closestPt) <= pow(sphereRadius, 2);
}

bool CollisionManager::AABBCollision(const vec3& minA, const vec3& maxA, const vec3& minB, const vec3& maxB)
{
	return
		(minA.x < maxB.x and maxA.x > minB.x) and
		(minA.y < maxB.y and maxA.y > minB.y) and
		(minA.z < maxB.z and maxA.z > minB.z);
}
