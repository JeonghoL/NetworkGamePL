#pragma once

#include <memory>
#include <unordered_map>

#include "GameObject.h"
#include "Character.h"

template<typename T>
concept ObjT = std::is_base_of_v<GameObject, T>;

class ObjectManager {
public:
	template<ObjT T, typename... Args>
	T* CreatObject(Args&&... args)
	{
		auto obj = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = obj.get();
		_objects[obj->GetId()] = std::move(obj);
		return ptr;
	}

	void Remove(int id) { _objects.erase(id); }
	void Update(const float dT)
	{
		for (auto& [id, obj] : _objects)
		{
			if (obj->IsAlive())
				obj->Update(dT);
		}
	}

	const auto& GetObjects() const { return _objects; }

private:
	std::unordered_map<int, std::unique_ptr<GameObject>> _objects;
};

