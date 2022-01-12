/************************************************************
*  �ļ�����TalkServerService.cpp
*  ������  ������ҵ��ʵ��
*************************************************************/
#include "MyTCP.h"
#include "MyTools.h"
#include "TalkServerService.h"

struct ClientInfo cinfo[10];  //�ͻ�����Ϣ��¼��

							  /**   ���������� */
void startServer()
{
	DWORD threadId;

	if (initSock(1)<0) //�����������׽���
		exit(0);

	//����һ���߳�ר���ڽ��ܿͻ��˵�����    
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myAcceptThread, NULL, 0, &threadId);
	return;
}

void myAcceptThread()
{
	DWORD threadId;
	printMsg("��ʼmyAcceptThread�߳�.........\n");

	while (1)
	{
		printMsg("�ȴ��ͻ�������....\n");
		int cs = tcpAccept();
		//����һ���������Ժ󴴽�һ���߳�ר�Ž��ո����ӵ�����
		int ind = findIdelCinfo();
		if (ind == -1) {//���������
			continue;
		}

		cinfo[ind].isFree = 1; //�Ѿ�ռ�ñ�־
		cinfo[ind].sock = cs;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myRecvThread, &cinfo[ind], 0, &threadId);
	}

	printMsg("�˳�Accept�߳�.........\n");
	return;
}

//���ݴ�����׽��ֽ�������
void myRecvThread(ClientInfo *pcinfo)
{
	char buf[1024] = { 0 };

	sprintf(buf, "��ʼmyRecvThread�߳�:%d.........\n", pcinfo->threadId);
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
	sprintf(buf, "����myRecvThread�߳�:%d.........\n", pcinfo->threadId);
	printMsg(buf);
}


void procLoginMsg(struct ClientInfo * pcinfo, struct LoginMsg *msg)
{
	strcpy(pcinfo->qq, msg->qq);  //����qq��������������

								  //���͵�¼�ɹ���Ϣ
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
	res.isOK = 0;  //û���ҵ�

	for (i = 0;i<sizeof(cinfo) / sizeof(struct ClientInfo); i++)
	{
		if (cinfo[i].isFree == 0) continue;
		if (strcmp(cinfo[i].qq, msg->qq) == 0)
		{
			res.isOK = 1; //��������
		}
	}

	//���ͺ���QQ���ҽ����Ϣ
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
			//��������Ϣת��������
			tcpSend(cinfo[i].sock, (const char *)pmsg, sizeof(struct TalkMsg));
			return;
		}
	}

	//���ѿ��������ˣ�����Ĵ�����������
	//����������������Ŀͻ��˷��ͺ��Ѳ����ߵ���Ϣ
}

/** �ڿͻ�����Ϣ��¼�����ҿ��е�Ԫ���±� **/
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
	printMsg("\t�����б��ͽ���\n");
}