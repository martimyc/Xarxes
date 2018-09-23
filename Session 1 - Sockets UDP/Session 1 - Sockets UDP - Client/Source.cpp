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

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET; // IPv4
	bindAddr.sin_port = htons(PORT); //Port
	const char *remoteAddrStr = "127.0.0.1"; // Not so remote… :-P
	inet_pton(AF_INET, remoteAddrStr, &bindAddr.sin_addr);

	/*int enable = 1;
	int res = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&enable, sizeof(int));
	if (res == SOCKET_ERROR) {
		OutputDebugString("Can not reuse socket");
	}

	int res = bind(s, (const sockaddr *)&bindAddr, sizeof(bindAddr));	*/	const char buf [5] = "Ping";	char* recived_buf = nullptr;
	int recived_len = 0;

	sockaddr_in recivedAddr = bindAddr;
	int recived_addr_len = sizeof(sockaddr_in);

	for (int i = 0; i < 5; i++)
	{
		sendto(s, buf, 5, 0, (const sockaddr *)&bindAddr, sizeof(bindAddr));
		recvfrom(s, recived_buf, recived_len, 0, (sockaddr *)&recivedAddr, &recived_addr_len);
	}

	iResult = WSACleanup();
	if (iResult != NO_ERROR)
	{
		OutputDebugString("Error with WSA clean up");
		return false;
	}
}