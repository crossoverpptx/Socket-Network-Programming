/************************************************************
*  �ļ�����TalkClientService.cpp
*  ������  �ͻ���ҵ��ʵ��
*************************************************************/
#include "MyTCP.h"
#include "MyTools.h"
#include "TalkClientService.h"
void myRecvThread();
#define PORT 5001


extern char serverip[6];

char qq[6] = { 0 };
char fqq[6] = { 0 };
int sock;

int status = STUS_START;

void mainProc()
{
	while (1)
	{

		switch (status)
		{
		case STUS_CONNECTED:
			statusConnected();
			break;

		case STUS_WAIT_LOGIN_RES:
			printMsg("�ȴ���������Ӧ..........\n");
			Sleep(1000);
			break;

		case STUS_LOGINED:
			statusLogined();
			break;

		case STUS_WAIT_QUERY_FQQ_RES:
			printMsg("�ȴ���������Ӧ..........\n");
			Sleep(1000);
			break;

		case STUS_TALK:
			statusTalk();
			break;
		case STUS_WAIT_QUERY_ALLQQ_RES:
			printMsg("�ȴ���������Ӧ..........\n");
			Sleep(1000);
			break;
		}
	}
}

void statusTalk()
{
	printMsg("�˳�����(Exit) ��˵>>");
	setbuf(stdin, NULL);   //��ռ��̻�����

	char buf[1024] = { 0 };
	gets_s(buf);
	if (strcmp(buf, "Exit") == 0)
	{
		printMsg("�ͻ����Ѿ��˳���\n");
		statusLogined();
	}

	else
	{
		struct TalkMsg msg;

		msg.id = TALK_MSG;
		strcpy(msg.qq, qq);
		strcpy(msg.fqq, fqq);
		strcpy(msg.info, buf);
		status = 5;
		tcpSend(sock, (const char*)&msg, sizeof(msg));
	}

	return;
}

void statusLogined()
{
	char buf0[10] = { 0 };
	int rt = 0;
	while (rt == 0)
	{
		memset(buf0, 0, sizeof(buf0));
		printMsg("��ʾ���ߺ���(L)|�Һ�������(T)|�˳�(E) Ĭ��(T)>>");
		setbuf(stdin, NULL);   //��ռ��̻�����
		gets_s(buf0);
		if (strcmp(buf0, "T") == 0 || strcmp(buf0, "t") == 0)
		{
			printMsg("ѡ���������......\n");
			while (1)
			{
				printMsg("�����������QQ��(��಻����5λ)��  �˳������롰quit��");
				char tmp[1024];
				scanf("%s", tmp);
				if (strlen(tmp) > 5)
				{
					printMsg("�������qq�ų��ȴ���5��������������\n");
				}
				else if (strcmp(tmp, qq) == 0)
				{
					printMsg("�������qq�����Լ����룬�ѵ���Ҫ����������::) ����������\n");
				}
				else if (strcmp(tmp, "quit") == 0)
					break;
				else
				{   //����Ҫ����ĺ���QQ�Ÿ�������
					struct FriendQqMsg msg;
					msg.id = QUERY_FQQ;
					strcpy(msg.qq, tmp);
					strcpy(fqq, tmp);
					status = STUS_WAIT_QUERY_FQQ_RES;
					tcpSend(sock, (const char*)&msg, sizeof(msg));
					rt = 1;
					break;
				}
			}
		}
		else if (strcmp(buf0, "L") == 0 || strcmp(buf0, "l") == 0)
		{
			char buf2[10] = { 0 };
			memset(buf2, 0, sizeof(buf2));
			struct RequestAllFriend res;
			res.id = REQUEST_ALL_FRD;
			memcpy(buf2, &res, sizeof(res));
			tcpSend(sock, (const char*)buf2, sizeof(buf2));
			status = STUS_WAIT_QUERY_ALLQQ_RES;
			Sleep(100);
		}
		else if (strcmp(buf0, "E") == 0 || strcmp(buf0, "e") == 0)
		{
			printMsg("�ͻ����Ѿ��˳���\n");
			exit(0);
		}
		else
		{
			printMsg("�������ݴ����ֶ��˰�-_-)������������\n");

		}


	};

}

void connectServer()
{
	printMsg("��ʼ���ӷ�����......\n");
	initSock(0);  //�ͻ���
	sock = tcpConnect(serverip, PORT);
	printMsg("���ӷ������ɹ���\n");

	//����һ���߳����ڽ��շ��������͵���Ϣ
	DWORD threadId;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myRecvThread, NULL, 0, &threadId);
}

//���������߳�ר���ڽ��ղ�������������ص�������
void myRecvThread()
{
	char buf[1024] = { 0 };
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		tcpRecv(sock, buf, sizeof(buf));
		unsigned char id = (unsigned char)buf[0];
		switch (id)
		{
		case LOGIN_MSG_RES:
			procLoginMsgResponse((struct LoginMsgResponse*)buf);
			break;

		case QUERY_FQQ_RES:
			procFriendQqMsgResponse((struct FriendQqMsgResponse*)buf);
			break;

		case TALK_MSG:
			procTalkMsg((struct TalkMsg*)buf);
			break;
		case APPLY_ALL_FRD:
			procApplyAllFriend((struct AllFriendMsg*)buf);
			break;

		}
	}
	sprintf(buf, "����myRecvThread�߳�:%d.........\n");
	printMsg(buf);
	exit(0);
}

void statusConnected()
{
	printMsg("\n\t�������¼��Ϣ\n");

	while (1)
	{
		printMsg("������QQ��(��಻����5λ����)��");
		char tmp[1024];
		scanf("%s", tmp);
		int  i;
		if (strlen(tmp) > 5)
		{
			printMsg("�������qq�ų��ȴ���5��������������\n");
			continue;
		}

		for (i = 0; i < strlen(tmp); i++)
		{
			if (tmp[i] >= '0' && tmp[i] <= '9')
			{
				continue;
			}
			else
				break;
		}

		if (i == strlen(tmp))
		{
			strcpy(qq, tmp);
			break;

		}
		else
		{
			printMsg("������Ϣ��������������\n");
			continue;
		}

	}

	struct LoginMsg msg;
	msg.id = LOGIN_MSG;
	status = STUS_WAIT_LOGIN_RES;
	strcpy(msg.qq, qq);
	tcpSend(sock, (const char*)&msg, sizeof(msg));
	mainProc(); //�������������
	return;
}

void procLoginMsgResponse(struct LoginMsgResponse* msg)
{
	if (msg->isOK == 1)
	{
		printMsg("��¼�ɹ���\n");
		status = STUS_LOGINED;//�ѵ�¼״̬
	}
	else
	{
		printMsg("��¼ʧ�ܣ�\n");
		printMsgExit(msg->reason);
		printf("\n");
	}
}

void procFriendQqMsgResponse(struct FriendQqMsgResponse* msg)
{
	if (msg->isOK == 1)
	{
		printMsg("����Ŀǰ���ߣ����Կ�ʼ����...\n");
		status = STUS_TALK;
	}
	else
	{
		printMsg("���Ѳ����ڰ����ٻ��������ԣ�\n");
		status = STUS_LOGINED;
	}
}

void procTalkMsg(struct TalkMsg* msg)
{
	char buf[1024] = { 0 };

	if (strcmp(msg->qq, fqq) == 0)
	{//��ʾ���Լ���������죬������ʾ��Ϣ����
		printf("\n");
		sprintf(buf, "%s˵>>%s", fqq, msg->info);
		printMsg(buf);
	}
	else
	{//��İ���˷��������
		printf("\n");
		sprintf(buf, "%s˵>>%s", msg->qq, msg->info);
		printMsg(buf);
	}
}

void procApplyAllFriend(struct AllFriendMsg* msg)
{

	char buf[100] = { 0 };
	sprintf(buf, "%s", msg->qq);
	printMsg("���ߵ�QQ���ѣ�   \n");
	printMsg(buf);
	printf("\n");
	status = STUS_LOGINED;
}