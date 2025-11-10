#pragma once

constexpr short PORT_NUM{ 9000 };

enum PacketType : char {
	// TODO
	CS_LOGIN,
};

enum MoveDirection : char {
	// TODO
};

#pragma pack(push, 1)

struct CS_LOGIN_PACKET {
	unsigned char size;
	char type;
	int textureId;
};

#pragma pack(pop)