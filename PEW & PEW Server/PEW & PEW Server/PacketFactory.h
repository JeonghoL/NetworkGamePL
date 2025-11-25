#pragma once

#include <vector>
#include <concepts>
#include "protocol.h"

class GameObject;

struct PacketFactory {
	static std::vector<char> SCLoginPacket();
	static std::vector<char> SCMovePacket(GameObject* obj);
	static std::vector<char> SCAddPacket(GameObject* obj);
	static std::vector<char> SCRemovePacket(GameObject* obj);

	template<typename Packet>
	static std::vector<char> Serialize(const Packet& packet)
	{
		static_assert(std::is_trivially_copyable_v<Packet>);

		std::vector<char> out(sizeof(Packet));
		std::memcpy(out.data(), &packet, sizeof(Packet));

		return out;
	}

	template<typename Packet>
	static Packet Deserialize(const std::vector<char>& buf)
	{
		static_assert(std::is_trivially_copyable_v<Packet>);

		if (buf.size() < sizeof(Packet)) return Packet{};

		Packet packet{};
		std::memcpy(&packet, buf.data(), sizeof(Packet));

		return packet;
	}
};

