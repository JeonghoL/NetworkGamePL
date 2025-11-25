#pragma once

#include <vector>
#include <concepts>
#include "protocol.h"

#include "Character.h"
#include "Projectile.h"

struct PacketFactory {
	static std::vector<char> SCLoginPacket();
	static std::vector<char> SCMovePacket(const Character& character);
	static std::vector<char> SCMovePacket(const Projectile& projectile);
	static std::vector<char> SCAddPacket(const Character& character);
	static std::vector<char> SCAddPacket(const Projectile& projectile);
	static std::vector<char> SCRemovePacket(const Character& character);
	static std::vector<char> SCRemovePacket(const Projectile& projectile);
	static std::vector<char> SCStatUpdatePacket(const Character& character);
	static std::vector<char> SCAttackPacket(const Character& character);
	static std::vector<char> SCAttackEndPacket(const Character& character);
	static std::vector<char> SCDeadPacket(const Character& character);
	static std::vector<char> SCRevivePacket(const Character& character);
	static std::vector<char> SCGameStartPacket();
	static std::vector<char> SCGameWinPacket();
	static std::vector<char> SCGameLosePacket();

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

