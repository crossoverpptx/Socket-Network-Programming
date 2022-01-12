/************************************************************
*  文件名：TalkServerService.cpp
*  描述：  服务器业务实现
*************************************************************/
#include "MyTCP.h"
#include "MyTools.h"
#include "TalkServerService.h"

struct ClientInfo cinfo[10];  //客户端信息记录区

							  /**   启动服务器 */
void startServer()
{
	DWORD threadId;

	if (initSock(1)<0) //创建服务器套接字
		exit(0);

	//创建一个线程专用于接受客户端的连接    
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myAcceptThread, NULL, 0, &threadId);
	return;
}

void myAcceptThread()
{
	DWORD threadId;
	printMsg("开始myAcceptThread线程.........\n");

	while (1)
	{
		printMsg("等待客户端连接....\n");
		int cs = tcpAccept();
		//当有一个连接上以后创建一个线程专门接收该连接的数据
		int ind = findIdelCinfo();
		if (ind == -1) {//服务器溢出
			continue;
		}

		cinfo[ind].isFree = 1; //已经占用标志
		cinfo[ind].sock = cs;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myRecvThread, &cinfo[ind], 0, &threadId);
	}

	printMsg("退出Accept线程.........\n");
	return;
}

//根据传入的套接字接收数据
void myRecvThread(ClientInfo *pcinfo)
{
	char buf[1024] = { 0 };

	sprintf(buf, "开始myRecvThread线程:%d.........\n", pcinfo->threadId);
	printMsg(buf);

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		int len = tcpRecv(pcinfo->sock, buf, sizeof(buf));
		if (len <0) break;

		unsigned char id = (unsigned char)buf[0];
		switch (id)
		{
		case LOGIN_MSG:
			procLoginMsg(pcinfo, (struct LoginMsg *)buf);
			break;

		case QUERY_FQQ:
			procFriendQqMsg(pcinfo, (struct FriendQqMsg *)buf);
			break;

		case TALK_MSG:
			procTalkMsg((struct TalkMsg *)buf);
			break;

		case REQUEST_ALL_FRD:
			procFriendQqMsgAll(pcinfo, (struct RequestAllFriend *) buf);
			break;
		}
	}

	pcinfo->isFree = 0;
	sprintf(buf, "结束myRecvThread线程:%d.........\n", pcinfo->threadId);
	printMsg(buf);
}


void procLoginMsg(struct ClientInfo * pcinfo, struct LoginMsg *msg)
{
	strcpy(pcinfo->qq, msg->qq);  //保存qq到服务器缓冲区

								  //发送登录成功消息
	struct LoginMsgResponse res;
	res.id = LOGIN_MSG_RES;
	res.isOK = 1;

	tcpSend(pcinfo->sock, (const char *)&res, sizeof(res));
}

void procFriendQqMsg(struct ClientInfo * pcinfo, struct FriendQqMsg *msg)
{
	int i;

	struct FriendQqMsgResponse res;
	res.id = QUERY_FQQ_RES;
	res.isOK = 0;  //没有找到

	for (i = 0;i<sizeof(cinfo) / sizeof(struct ClientInfo); i++)
	{
		if (cinfo[i].isFree == 0) continue;
		if (strcmp(cinfo[i].qq, msg->qq) == 0)
		{
			res.isOK = 1; //好友在线
		}
	}

	//发送好友QQ查找结果消息
	tcpSend(pcinfo->sock, (const char *)&res, sizeof(res));
}

void procTalkMsg(struct TalkMsg *pmsg)
{
	int i;

	for (i = 0;i<sizeof(cinfo) / sizeof(struct ClientInfo); i++)
	{
		if (cinfo[i].isFree == 0) continue;

		if (strcmp(cinfo[i].qq, pmsg->fqq) == 0)
		{
			//将聊天消息转发给好友
			tcpSend(cinfo[i].sock, (const char *)pmsg, sizeof(struct TalkMsg));
			return;
		}
	}

	//好友可能下线了，下面的代码请你完善
	//服务器给发起聊天的客户端发送好友不在线的消息
}

/** 在客户端信息记录区查找空闲的元素下标 **/
int findIdelCinfo()
{
	int i;
	for (i = 0;i<sizeof(cinfo) / sizeof(struct ClientInfo); i++)
	{
		if (cinfo[i].isFree == 0)
		{
			return i;
		}
	}

	return -1;
}

void procFriendQqMsgAll(struct ClientInfo * pcinfo, struct RequestAllFriend *msg)
{
	struct AllFriendMsg res;
	res.id = APPLY_ALL_FRD;
	int i;
	memset(res.qq, 0, sizeof(res.qq));

	for (i = 0;i < sizeof(cinfo) / sizeof(struct ClientInfo);i++)
	{
		if (cinfo[i].isFree == 0)
			continue;
		else if (cinfo[i].isFree == 1)
		{
			if (strcmp(pcinfo->qq, cinfo[i].qq) == 0) 
				continue;
			sprintf(res.qq, "%s%s,", res.qq, cinfo[i].qq);
		}	
	}
    res.qq[strlen(res.qq)-1] = '\0';
	tcpSend(pcinfo->sock, (char *)&res, sizeof(res));
	printMsg("\t好友列表发送结束\n");
}