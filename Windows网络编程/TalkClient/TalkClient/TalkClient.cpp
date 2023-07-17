/************************************************************
*  �ļ�����TalkClient.cpp
*  ������  �ͻ�����ڣ��������
*************************************************************/
#include <stdio.h>
#include <Windows.h>

#include "TalkClientService.h"

void displayMenu();
void exitProc();
void defProc();
void procMenu();

char serverip[6];

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("�������������IP��ַ������\n");
		return 0;
	}
	strcpy(serverip, argv[1]);
	displayMenu();
	procMenu();
	while (1) Sleep(10000);
}

char ch;     //�˵�ѡ��
void displayMenu()
{
	static int t = 1;
	if (t++ == 1)
	{
		printf("\n\n\n\n\n");
		printf("\n\t      @                                                             @        ");
		printf("\n\t    @ ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ @      ");
		printf("\n\t   @  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   @    ");
		printf("\n\t  @   **      **  **********  **        **         ********   **  **     @   ");
		printf("\n\t @    **      **  **          **        **        **      **  **  **      @  ");
		printf("\n\t @    **      **  **          **        **        **      **  **  **       @  ");
		printf("\n\t @    **********  **********  **        **        **      **  **  **       @  ");
		printf("\n\t @    **      **  **          **        **        **      **  **  **       @  ");
		printf("\n\t @    **      **  **          **        **        **      **              @  ");
		printf("\n\t  @   **      **  **********  ********  ********   ********   @@  @@     @   ");
		printf("\n\t   @   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  @    ");
		printf("\n\t    @  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^@      ");
		printf("\n\t      @                                                             @        ");

		printf("\n                                                             -----By Hins    ");
		printf("\n\n\n\n\n\t\tPress 'Enter' to enter the menu......");           /*����һ���������˵�*/
	}
	getchar();                                 /*�Ӽ��̶�ȡһ���ַ�,������ʾ����Ļ*/
	system("cls"); /*����*/
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\t************************************************\n");
	printf("\t*                                              *\n");
	printf("\t*         �Ϲ�2016����ͻ��˳���               *\n");
	printf("\t*         Version 2.1     ������               *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*           1) ��¼(L)                         *\n");
	printf("\t*           2) �˳�(e)                         *\n");
	printf("\t*                      by---HinsShwan          *\n");
	printf("\t************************************************\n");
	printf("\n");
	connectServer();
	printf("\t����ѡ����:");
	ch = getchar();
}

void procMenu()
{
	switch (ch)
	{
	case 'l':
	case 'L':
	case '1':
		statusConnected();
		break;

	case 'e':
	case 'E':
	case '2':
		exitProc();
		break;

	default:
		defProc();
	}
}

void exitProc()
{
	printf("\t�ͻ��˳����Ѿ���ֹ��\n");
	exit(0);
}

void defProc()
{
	printf("\t �����ַ�����\n");
}