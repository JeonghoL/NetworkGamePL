#pragma once

#include <iostream>

enum PacketType : uint8_t {
	PT_TEMP = 0,
	SC_LOGIN = 1,
};

#pragma pack(push, 1)

struct PacketHeader {
	PacketType type;
	uint8_t    size;

	PacketHeader(PacketType t, uint8_t s)
		: type(t), size(s) {}
};

struct TempPacket : public PacketHeader {
	float x, y, z;

	TempPacket(float x, float y, float z)
		: PacketHeader(PacketType::PT_TEMP, sizeof(TempPacket)), 
		x(x), y(y), z(z) {}
}; 

struct SC_LOGIN_PACKET : public PacketHeader {
	SC_LOGIN_PACKET() 
		: PacketHeader(PacketType::SC_LOGIN, sizeof(SC_LOGIN_PACKET)) {}
};

#pragma pack(pop)