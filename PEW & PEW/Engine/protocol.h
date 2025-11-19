#pragma once

constexpr short PORT_NUM{ 9000 };

enum PacketType : char {
	CS_LOGIN,
	CS_MOVE,

	SC_LOGIN,
	SC_MOVE_OBJECT,
	SC_ADD,
	SC_REMOVE
};

enum MoveDirection : char {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT
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

struct SC_ADD_PACKET {
	unsigned char size;
	char type;
	int id;
	float x;
	float y;
	float z;
};

struct SC_REMOVE_PACKET {
	unsigned char size;
	char type;
	int id;
};

#pragma pack(pop)