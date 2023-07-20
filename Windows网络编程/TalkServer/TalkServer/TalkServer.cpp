/************************************************************
*  �ļ�����TalkServer.cpp
*  ������  ��������ڣ��û���������ģ��
*************************************************************/

#include <stdio.h>
#include "TalkServerService.h"

extern struct ClientInfo cinfo[10];  //�ͻ�����Ϣ��¼��

void procMenu();
void exitProc();
void defProc();
void displayMenu();

void procMenu()
{
	while (1)
	{


		char ch;     //�˵�ѡ��
		char buf[200];
		fgets(buf, sizeof(buf), stdin);
		ch = buf[0];
		printf("\n");
		switch (ch)
		{
		case 's':
		case 'S':
		case '1':
			startServer();
			break;

		case '2':
		case 'u':
		case 'U':
			showallmessage();
			break;

		case 'e':
		case 'E':
		case '3':
			exitProc();
			break;

		default:
			defProc();
		}
	}
}

void exitProc()
{
	printf("\tϵͳ�Ѿ��˳���\n");
	exit(0);
}

void defProc()
{
	printf("\t�����ַ�����\n");
}

void showallmessage()
{
	int i = 0;
	printf("\t���Ϲ�����������������ѵ����������QQΪ:\n");
	for (i = 0; i < sizeof(cinfo) / sizeof(struct ClientInfo); i++)
	{
		if (cinfo[i].isFree == 0)
			continue;
		else
		{
			printf("\t  %s\n", cinfo[i].qq);
		}
	}
}

/*  ϵͳ�˵�  */
void displayMenu()
{
	printf("\t************************************************\n");
	printf("\t*                                              *\n");
	printf("\t*    �Ϲ��������������                        *\n");
	printf("\t*              V1.0                            *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*      1) ���� (s)                             *\n");
	printf("\t*      2) ��ʾ��¼�û�(u)                      *\n");
	printf("\t*      3) �˳�(e)                              *\n");
	printf("\t*                                              *\n");
	printf("\t************************************************\n");
	printf("\n");
	printf("\t����ѡ����:");

}

int main(int agrc, char* argv[])
{
	displayMenu();
	procMenu();
}
