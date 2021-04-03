#include <stdio.h>
#include <WinSock2.h>

int SendData(SOCKET c, char* data, int len);
int RecvData(SOCKET c, char** buffer);

int main()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf("Khong tao duoc socket, ma loi: %d\n", WSAGetLastError());
		return 1;
	}
	else
	{
		SOCKADDR_IN saddr;
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(8888);
		saddr.sin_addr.S_un.S_addr = INADDR_ANY;
		bind(s, (sockaddr *)&saddr, sizeof(saddr));
		listen(s, 10);
		SOCKADDR_IN caddr;
		int clen = sizeof(caddr);
		SOCKET c = accept(s, (sockaddr*)&caddr, &clen);
		char* hello = (char*)"Hello\n";
		int byteSent = SendData(c, hello, strlen(hello));
		printf("So byte da gui: %d\n", byteSent);
		char* data = NULL;
		int byteReceived = RecvData(c, &data);
		printf("So byte da nhan: %d\n", byteReceived);
		printf("Du lieu nhan duoc: %s\n", data);
		free(data);
		closesocket(c);
		closesocket(s);
		WSACleanup();
	}
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
	} 	while (byteReceived == MAX);
	return totalReceived;
}