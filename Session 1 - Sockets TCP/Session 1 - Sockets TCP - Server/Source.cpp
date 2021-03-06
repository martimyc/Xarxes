#define WIN32_LEAN_AND_MEAN
#define NOMIMAX

#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8000

int main(int argc, char** argv)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		OutputDebugString("Error initializing WSA");
		return false;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET; // IPv4
	bindAddr.sin_port = htons(PORT); //Port
	bindAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	int enable = 1;
	int res = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&enable, sizeof(int));
	if (res == SOCKET_ERROR) {
		OutputDebugString("Can not reuse socket");
	}

	res = bind(s, (const sockaddr *)&bindAddr, sizeof(bindAddr));	if (res == SOCKET_ERROR) {
		OutputDebugString("Can not bind socket");
	}	const char* buf = "Pong";	char recived_buf[5];
	int recived_len = 5;

	sockaddr_in recivedAddr = bindAddr;
	int recived_addr_len = sizeof(sockaddr_in);

	listen(s, 1);

	SOCKET s2 = accept(s, (sockaddr *)&recivedAddr, &recived_addr_len);

	for (int i = 0; i < 5; i++)
	{
		recv(s2, recived_buf, recived_len, 0);
		send(s2, buf, strlen(buf), 0);
		OutputDebugString(recived_buf);
		OutputDebugString(buf);
	}

	iResult = WSACleanup();
	if (iResult != NO_ERROR)
	{
		OutputDebugString("Error with WSA clean up");
		return false;
	}
}