#include "pch.h"
#include "NetworkManager.h"

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
}

bool NetworkManager::IsConnected() const
{
	return isConnected;
}