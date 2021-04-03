#include <stdio.h>
#include <WS2tcpip.h>
#include <WinSock2.h>

int SendData(SOCKET c, char* data, int len);
int RecvData(SOCKET c, char** buffer);

int main()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET c = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(7777);
	/*char addr[INET_ADDRSTRLEN]; 
	inet_pton(AF_INET, "127.0.0.1", &addr);*/
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//(ULONG) addr;
	int result = connect(c, (sockaddr*)&saddr, sizeof(saddr));
	if (result != SOCKET_ERROR)
	{
		char* hello = (char*)"Hello World\n";
		SendData(c, hello, strlen(hello));
		char* buffer = NULL;
		RecvData(c, &buffer);
		printf("%s\n", buffer);
		free(buffer);
		buffer = NULL;
		closesocket(c);
	}
	WSACleanup();
}

int SendData(SOCKET c, char* data, int len)
{
	int byteSent = 0;
	while (byteSent < len)
	{
		byteSent += send(c, data + byteSent, len - byteSent, 0);
	}
	return byteSent;
}

int RecvData(SOCKET c, char** buffer)
{
	const int MAX = 1024;
	int totalReceived = 0;
	char tmp[MAX];
	int byteReceived = 0;
	do
	{
		byteReceived = recv(c, tmp, MAX, 0);
		if (byteReceived > 0)
		{
			*buffer = (char*)realloc(*buffer, totalReceived + byteReceived + 1);
			memset(*buffer + totalReceived, 0, byteReceived + 1);
			memcpy(*buffer + totalReceived, tmp, byteReceived);
			totalReceived += byteReceived;
		}
	} while (byteReceived == MAX);
	return totalReceived;
}