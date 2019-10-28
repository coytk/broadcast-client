//#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

const int MAX_BUF_LEN = 255;

int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// 启动socket api
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	// 创建socket
	SOCKET connect_socket;
	connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connect_socket)
	{
		err = WSAGetLastError();
		printf("/socket /  error! error code is %d/n", err);
		return -1;
	}

	// 用来绑定套接字
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(3779);
	sin.sin_addr.s_addr = 0;

	// 用来从网络上的广播地址接收数据
	SOCKADDR_IN sin_from;
	sin_from.sin_family = AF_INET;
	sin_from.sin_port = htons(3779);
	sin_from.sin_addr.s_addr = INADDR_BROADCAST;

	//设置该套接字为广播类型，
	bool bOpt = true;
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));

	// 绑定套接字
	err = bind(connect_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR));
	if (SOCKET_ERROR == err)
	{
		err = WSAGetLastError();
		printf("/bind /  error! error code is %d/n", err);
		return -1;
	}

	int nAddrLen = sizeof(SOCKADDR);
	char buff[MAX_BUF_LEN] = "";
	int nLoop = 0;
	while (1)
	{
		// 接收数据
		int nSendSize = recvfrom(connect_socket, buff, MAX_BUF_LEN, 0, (SOCKADDR*)&sin_from, &nAddrLen);
		if (SOCKET_ERROR == nSendSize)
		{
			err = WSAGetLastError();
			printf("/recvfrom /  error! error code is %d/n", err);
			return -1;
		}
		buff[nSendSize] = '\0';
		printf("Recv: %s\n", buff);
	}

	return 0;
}