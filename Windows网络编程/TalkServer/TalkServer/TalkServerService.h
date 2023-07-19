/************************************************************
*  �ļ�����TalkServerService.h
*  ������  ������ҵ��㺯��������
*************************************************************/

#ifndef _TALKSERVERSERVICE_H_
#define _TALKSERVERSERVICE_H_

#include <windows.h> // �⺯�����壬��system��pause��
#include <stdlib.h>
#include <stdio.h>
#include "MyMsg.h"

struct ClientInfo
{
	char qq[6];              //�ͻ���ID��
	int sock;                //�ͻ����Ѿ��������ӵ��׽���
	unsigned char isFree;    //���б�־
	unsigned int threadId;   //Ϊ����ÿͻ��˽����Ľ����߳�ID
};

void procFriendQqMsgAll(struct ClientInfo* pcinfo, struct RequestAllFriend* msg);
void startServer();
void myAcceptThread();
void myRecvThread(ClientInfo* pinfo);
void procLoginMsg(struct ClientInfo* pcinfo, struct LoginMsg* msg);
void procFriendQqMsg(struct ClientInfo* pcinfo, struct FriendQqMsg* msg);
void procTalkMsg(struct TalkMsg* msg);
int findIdelCinfo();
void showallmessage();

#endif
