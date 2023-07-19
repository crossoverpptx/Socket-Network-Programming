/************************************************************
*  文件名：TalkServerService.h
*  描述：  服务器业务层函数申明等
*************************************************************/

#ifndef _TALKSERVERSERVICE_H_
#define _TALKSERVERSERVICE_H_

#include <windows.h> // 库函数定义，如system、pause等
#include <stdlib.h>
#include <stdio.h>
#include "MyMsg.h"

struct ClientInfo
{
	char qq[6];              //客户端ID号
	int sock;                //客户端已经建立连接的套接字
	unsigned char isFree;    //空闲标志
	unsigned int threadId;   //为处理该客户端建立的接收线程ID
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
