#include "pch.h"
#include "NetworkManager.h"
#include "PacketFactory.h"
#include "GraphicsManager.h"
#include "MainCharacter.h"
#include "StaticObjectManager.h"

NetworkManager::NetworkManager()
{
	clientSocket = INVALID_SOCKET;
	isConnected = false;
}

NetworkManager::~NetworkManager()
{
	if (isConnected)
		Release();
}

void NetworkManager::Init(const char* IP, u_short port)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "Socket creation failed!" << endl;
		WSACleanup();
		return;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, IP, &serverAddr.sin_addr);

	if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		cout << "Connection failed!" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	// Socket을 Non-Blocking으로 변경
	u_long nonBlocking{ 1 };
	ioctlsocket(clientSocket, FIONBIO, &nonBlocking);

	isConnected = true;
	cout << "Connected to Server!" << endl;
}

void NetworkManager::Update()
{
	if (not isConnected) {
		return;
	}

	char tempBuffer[1024];
	int recvLen = recv(clientSocket, tempBuffer, sizeof(tempBuffer), 0);
	if (SOCKET_ERROR == recvLen) {
		int error = WSAGetLastError();
		if (WSAEWOULDBLOCK == error) {
			// 읽을 데이터 없음
			return;
		}

		else {
			// 단순 에러 상황
			std::cerr << "recv() error : " << error << std::endl;
			Release();
			return;
		}
	}

	else if (0 >= recvLen) {
		// 연결 종료
		std::cout << "Server DisConneted\n";
		Release();
		return;
	}

	else {
		// 정상 상황 / 패킷 재조립

		// 1. 받은 데이터 RecvBuffer에 추가
		recvBuffer.insert(recvBuffer.end(), tempBuffer, tempBuffer + recvLen);

		// 2. 패킷 재조립
		while (true) {
			// 2-1. 최소 패킷 크기 확인
			//      현재 패킷의 size를 unsigned char로 받고 있음
			if (recvBuffer.size() < sizeof(unsigned char)) {
				break;
			}

			// 2-2. 패킷 size 추출
			unsigned char packetSize = static_cast<unsigned char>(recvBuffer[0]);

			// 2-3. 정상 패킷인지 확인
			//      패킷 사이즈 확인 (현재 Protocol의 최대 사이즈는 23인데 일단 넉넉하게 잡음)
			if (packetSize <= 0 or packetSize > 32) {
				std::cerr << "Invalid Packet Size : " << (int)packetSize << std::endl;
				recvBuffer.clear();
				break;
			}

			// 아직 전체 패킷이 오지 않았을 때
			if (recvBuffer.size() < packetSize) {
				break;
			}

			// 2-4. 정상적인 패킷이 모두 왔을 때
			//      패킷 추출해서 처리
			std::vector<char> packet(recvBuffer.begin(), recvBuffer.begin() + packetSize);
			recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + packetSize);

			ProcessPacket(packet);
		}

	}

}

void NetworkManager::Release()
{
	if (isConnected)
	{
		closesocket(clientSocket);
		isConnected = false;

		cout << "Socket closed!" << endl;
	}
	WSACleanup();
}

void NetworkManager::Send(const std::vector<char>& packet)
{
	if (SOCKET_ERROR == send(clientSocket, packet.data(), packet.size(), 0)) {
		int error = WSAGetLastError();
		if (WSAEWOULDBLOCK == error) {
			// 송신 버퍼 OverFlow -> 별도 처리 X (패킷 누락)
		}
		else {
			Release();
		}
	}
}

void NetworkManager::ProcessPacket(const std::vector<char>& packet)
{
	if (packet.size() < 2) return;

	char packetType = packet[1];

	switch (packetType) {
	case SC_ADD:
	{
		SC_ADD_PACKET addPacket = PacketFactory::Deserialize<SC_ADD_PACKET>(packet);

		if (graphics) {
			if (addPacket.id < 64) {
				// 첫 번째 받은 캐릭터를 내 캐릭터로 설정
				static bool firstCharacter = true;
				bool isLocal = firstCharacter;
				firstCharacter = false;

				glm::vec3 addCharacterPos = glm::vec3(addPacket.x, addPacket.y, addPacket.z);
				graphics->AddCharacter(addPacket.id, addCharacterPos, addPacket.textureId, isLocal, 20.0f);

				MainCharacter* character = graphics->GetCharacter(addPacket.id);
				if (character) {
					character->SetTargetPosition(addPacket.x, addPacket.y, addPacket.z);
				}

				std::cout << "[ADD PLAYER] ID: " << addPacket.id << " at ("
					<< addPacket.x << ", " << addPacket.y << ", " << addPacket.z << ")"
					<< (isLocal ? " (LOCAL)" : " (REMOTE)") << std::endl;
			}
			else
			{
				int ownerID = addPacket.ownerId;
				MainCharacter* character = graphics->GetCharacter(ownerID);
				if (character) {
					glm::vec3 startPos(addPacket.x, addPacket.y, addPacket.z);
					character->CreateBulletFromServer(addPacket.id, startPos);
				}
			}
		}
		break;
	}
	case SC_MOVE_OBJECT:
	{
		SC_MOVE_PACKET movePacket = PacketFactory::Deserialize<SC_MOVE_PACKET>(packet);

		if (graphics) {
			if (movePacket.id < 64) {
				// 캐릭터 이동 처리 (기존 코드)
				MainCharacter* character = graphics->GetCharacter(movePacket.id);
				if (character) {
					character->UpdateFromPacket(movePacket.angle, movePacket.x, movePacket.y, movePacket.z, -1, movePacket.isMove, movePacket.isRun);
				}
			}
			else {
				// 총알 이동 처리 - 모든 캐릭터에서 해당 총알 찾기
				glm::vec3 newPos(movePacket.x, movePacket.y, movePacket.z);
				bool bulletFound = false;

				// 모든 캐릭터를 순회하면서 해당 총알 ID 찾기
				for (auto& [id, character] : graphics->GetAllCharacters()) {
					if (character->SetBulletNextPosFromServer(movePacket.id, newPos)) {
						bulletFound = true;
						break;
					}
				}
			}
		}
		break;
	}
	case SC_REMOVE:
	{
		SC_REMOVE_PACKET removePacket = PacketFactory::Deserialize<SC_REMOVE_PACKET>(packet);

		if (graphics) {
			if (removePacket.id < 64) {
				graphics->RemoveCharacter(removePacket.id);
			}
			else
			{
				bool bulletFound = false;

				for (auto& [id, character] : graphics->GetAllCharacters()) {
					if (character->RemoveBulletFromServer(removePacket.id)) {
						bulletFound = true;
						break;
					}
				}
			}
		}
		break;
	}
	case SC_ATTACK:
	{
		SC_ATTACK_PACKET attackPacket = PacketFactory::Deserialize<SC_ATTACK_PACKET>(packet);

		if (graphics) {
			MainCharacter* character = graphics->GetCharacter(attackPacket.id);
			if (character) {
				character->SetFiring(true);
			}
		}
		break;
	}
	case SC_ATTACK_END:
	{
		SC_ATTACK_END_PACKET attackEndPacket = PacketFactory::Deserialize<SC_ATTACK_END_PACKET>(packet);

		if (graphics) {
			MainCharacter* character = graphics->GetCharacter(attackEndPacket.id);
			if (character) {
				character->SetFiring(false);
			}
		}
		break;
	}
	case SC_DEAD:
	{
		SC_DEAD_PACKET deadPacket = PacketFactory::Deserialize<SC_DEAD_PACKET>(packet);

		if (graphics) {
			MainCharacter* character = graphics->GetCharacter(deadPacket.id);
			if (character) {
				character->SetDying(true);
				cout << deadPacket.id << ": Dead!!" << '\n';
			}
		}
		break;
	}
	case SC_REVIVE:
	{
		SC_REVIVE_PACKET revivePacket = PacketFactory::Deserialize<SC_REVIVE_PACKET>(packet);

		if (graphics) {
			MainCharacter* character = graphics->GetCharacter(revivePacket.id);
			if (character) {
				character->SetDying(false);
				character->ReviveFromPacket(revivePacket.x, revivePacket.y, revivePacket.z);
				cout << revivePacket.id << ": Revived!!" << '\n';
			}
		}
		break;
	}
	case SC_STAT_UPDATE:
	{
		SC_STAT_UPDATE_PACKET statPacket = PacketFactory::Deserialize<SC_STAT_UPDATE_PACKET>(packet);

		if (graphics) {
			MainCharacter* character = graphics->GetCharacter(statPacket.id);
			if (character) {
				character->DamagedFromPacket();
			}
		}
		break;
	}
	case SC_GAME_START:
	{
		SC_GAME_START_PACKET startPacket = PacketFactory::Deserialize<SC_GAME_START_PACKET>(packet);

		canStart = true;
		break;
	}
	case SC_GAME_WIN:
	{
		GET_SINGLE(StaticObjectManager)->SetPlayerState(PlayerPVPState::WIN);
		break;
	}
	case SC_GAME_LOSE:
	{
		GET_SINGLE(StaticObjectManager)->SetPlayerState(PlayerPVPState::LOSE);
		break;
	}
	default:
		std::cout << "[UNKNOWN PACKET] Type: " << (int)packetType << std::endl;
		break;
	}
}

bool NetworkManager::IsConnected() const
{
	return isConnected;
}

bool NetworkManager::CanStart() const
{
	return canStart;
}
