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

	sockaddr_in remoteAddr;
	remoteAddr.sin_family = AF_INET; // IPv4
	remoteAddr.sin_port = htons(PORT); //Port
	const char *remoteAddrStr = "127.0.0.1"; // Not so remote… :-P
	inet_pton(AF_INET, remoteAddrStr, &remoteAddr.sin_addr);	const char buf[5] = "Ping";	char* recived_buf = nullptr;
	int recived_len = 0;

	sockaddr_in recivedAddr = remoteAddr;
	int recived_addr_len = sizeof(sockaddr_in);

	connect( s, (const sockaddr *)&remoteAddr, sizeof(remoteAddr));

	for (int i = 0; i < 5; i++)
	{
		send(s, buf, 5, 0);
		recv(s, recived_buf, recived_len, 0);
	}

	iResult = WSACleanup();
	if (iResult != NO_ERROR)
	{
		OutputDebugString("Error with WSA clean up");
		return false;
	}
}