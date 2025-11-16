#pragma once

#include <iostream>

enum PacketType : char {
	CS_LOGIN,


	SC_LOGIN
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