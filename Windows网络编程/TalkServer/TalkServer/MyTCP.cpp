/************************************************************
*  文件名：MyTCP.cpp
*  描述：  TCP/IP通信socket封装
*************************************************************/

#include <stdio.h>
#include <WinSock2.h> // Windows 网络编程接口定义，如bind函数等。如Linux中的 #include <sys/socket.h>
#include "MyTCP.h"
#include "MyTools.h"
#include <Ws2tcpip.h> // Windows 网络编程信息转换接口定义，如InetNtop，此函数的 ANSI 版本inet_ntop。如Linux中的 #include <arpa/inet.h>
#pragma comment (lib, "ws2_32.lib") // 链接Ws2_32.lib这个库，效果与在project->setting->link->Object/library Modules:在框里加入"***.lib"一样。

#define PORT 5001
int ls;   //侦听套接字
int flag; //记录服务器或是客户端的标志

/*   描述: 初始化服务器或客户端，该函数在使用中必需首先调用，客户端和服务器 */
int initSock(int IsServer)
{
	WSADATA data;

	if (WSAStartup(1, &data) < 0)
		printMsgExit("call WSAStartup() failure!");

	ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ls < 0)
		printMsgExit("创建套接字失败!");

	flag = IsServer;
	if (IsServer != 0)
	{//服务器
		struct sockaddr_in servAddr;
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(PORT);

		if (bind(ls, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
			printMsg("bind套接字失败!\n");
			return -1;
		}

		if (listen(ls, 10) < 0) {
			printMsg("listen套接字失败!\n");
			return -1;
		}
	}

	return 0;
}

/*  描述: 仅用于服务器, 等待服务器连接 */
int tcpAccept()
{
	struct sockaddr_in cliAddr;
	int cliAddrLen = sizeof(cliAddr);

	int s = accept(ls, (struct sockaddr*)&cliAddr, &cliAddrLen);
	if (s < 0)
	{
		printMsgExit("服务器Accept异常！");
	}

	char tmp[40] = { 0 };

	//sprintf_s(tmp, "客户端进入,IP为:%s\n", InetNtop(AF_INET, &cliAddr.sin_addr, tmp, 40));
	sprintf_s(tmp, "客户端进入,IP为:%s\n", inet_ntoa(cliAddr.sin_addr));
	printMsg(tmp);
	printInt(s);

	return s;
}

/* 描述: 发送数据 */
int tcpSend(unsigned int sock, const char* sendBuf, int sendBufLen)
{
	int len = send(sock, sendBuf, sendBufLen, 0);
	return len;
}

/* 描述: 接收数据 */
int tcpRecv(unsigned int sock, char* recvBuf, int recvBufLen)
{
	int len = recv(sock, recvBuf, recvBufLen, 0);
	return len;
}

void tcpClose(unsigned int sock)
{
	closesocket(sock);
	WSACleanup();
}
