#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")

int main()
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("socket() error\n");
        return 1;
    }

    // 서버 주소 설정
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // localhost
    serveraddr.sin_port = htons(9000);

    // 서버에 연결
    if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("connect() error\n");
        return 1;
    }

    // 데이터 통신
    char buf[512];
    while (1) {
        // 데이터 입력
        printf("보낼 데이터: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        if (strcmp(buf, "quit\n") == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        }

        // 데이터 보내기
        int retval = send(sock, buf, strlen(buf), 0);
        if (retval == SOCKET_ERROR) {
            printf("send() error\n");
            break;
        }
    }

    // 소켓 닫기
    closesocket(sock);
    WSACleanup();
    return 0;
}