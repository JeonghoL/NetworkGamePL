#pragma once

#include <iostream>

enum PacketType : char {
	CS_LOGIN,
	CS_MOVE,

	SC_LOGIN,
	SC_MOVE
};

#pragma pack(push, 1)

struct CS_LOGIN_PACKET {
	unsigned char size;
	char type;
	int textureId;
};

struct CS_MOVE_PACKET {
	unsigned char size;
	char type;
	float angle;
	char direction;
	bool isRun;
};

struct SC_LOGIN_PACKET {
	unsigned char size;
	char type;
};

struct SC_MOVE_PACKET {
	unsigned char size;
	char type;
	int id;
	float angle;
	float x;
	float y;
	float z;
	bool isMove;
	bool isRun;
};

#pragma pack(pop)