#pragma once

#include <vector>
#include <concepts>
#include "protocol.hpp"

template<typename T>
concept Packet = std::is_base_of_v<PacketHeader, T>;

struct PacketFactory {
	static std::vector<char> SCLoginPacket();


	template<Packet T>
	static std::vector<char> Serialize(const T& packet)
	{
		std::vector<char> out(sizeof(T));
		std::memcpy(out.data(), &packet, sizeof(T));

		return out;
	}

	template<Packet T>
	static T Deserialize(const std::vector<char>& buf)
	{
		if (buf.size() < sizeof(T)) return T{};

		T packet;
		std::memcpy(&packet, buf.data(), sizeof(T));

		return packet;
	}
};

