#pragma once

class PacketFactory
{
public:
	// Client -> Server
	static vector<char> CSLoginPacket(int characterType);
	static vector<char> CSMovePacket(float angle, char direction, bool run = false);

public:
	template<typename Packet>
	static vector<char> Serialize(const Packet& packet)
	{
		static_assert(std::is_trivially_copyable_v<Packet>);

		vector<char> out(sizeof(Packet));
		memcpy(out.data(), &packet, sizeof(Packet));

		return out;
	}

	template<typename Packet>
	static Packet Deserialize(const vector<char>& buf)
	{
		static_assert(std::is_trivially_copyable_v<Packet>);

		if (buf.size() < sizeof(Packet)) {
			return Packet{};
		}

		Packet packet{};
		memcpy(&packet, buf.data(), sizeof(Packet));

		return packet;
	}

};

