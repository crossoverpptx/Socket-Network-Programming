/************************************************************
*  �ļ�����MyTCP.cpp
*  ������  TCP/IPͨ��socket��װ
*************************************************************/
#include <stdio.h>
#include <WinSock2.h>
#include "MyTools.h"
#pragma comment (lib, "ws2_32.lib")

#define PORT 5001
int ls;   //�����׽���
int flag;//��¼���������ǿͻ��˵ı�־

/*   ����: ��ʼ����������ͻ���, �ú�����ʹ���б������ȵ���,�ͻ��˺ͷ����� */
int initSock(int IsServer)
{
	WSADATA data;

	if (WSAStartup(1, &data) < 0)
		printMsgExit("call WSAStartup() failure!");

	ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ls < 0)
		printMsgExit("�����׽���ʧ��!");

	flag = IsServer;
	if (IsServer != 0)
	{//������
		struct sockaddr_in servAddr;
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(PORT);

		if (bind(ls, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
			printMsg("bind�׽���ʧ��!\n");
			return -1;
		}

		if (listen(ls, 10) < 0) {
			printMsg("listen�׽���ʧ��!\n");
			return -1;
		}
	}

	return 0;
}

/*   ����: �����ڿͻ���, ���ӷ����� */
int tcpConnect(const char* serverIP, unsigned short port)
{
	struct sockaddr_in servAddr;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(serverIP);
	servAddr.sin_port = htons(port);

	if (connect(ls, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
	{
		printMsgExit("���ӷ�����ʧ�ܣ�\n");
	}

	return ls;
}

/* ����: �������� */
int tcpSend(unsigned int sock, const char* sendBuf, int sendBufLen)
{
	int len = send(sock, sendBuf, sendBufLen, 0);
	return len;
}

/* ����: �������� */
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