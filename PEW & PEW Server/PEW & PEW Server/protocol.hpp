#pragma once

#include <iostream>

enum PacketType : char {
	CS_LOGIN,


	SC_LOGIN
};

template<>
struct std::hash<PacketType>
{
	std::uint8_t operator()(const PacketType& p) const noexcept
	{
		return static_cast<std::uint8_t>(p);
	}
};

#pragma pack(push, 1)

struct CS_LOGIN_PACKET {
	unsigned char size;
	char type;
	int textureId;
};

struct SC_LOGIN_PACKET {
	unsigned char size;
	char type;
};

#pragma pack(pop)