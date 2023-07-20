/************************************************************
*  文件名：TalkServer.cpp
*  描述：  服务器入口，用户操作界面模块
*************************************************************/

#include <stdio.h>
#include "TalkServerService.h"

extern struct ClientInfo cinfo[10];  //客户端信息记录区

void procMenu();
void exitProc();
void defProc();
void displayMenu();

void procMenu()
{
	while (1)
	{


		char ch;     //菜单选择
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
	printf("\t系统已经退出！\n");
	exit(0);
}

void defProc()
{
	printf("\t输入字符错误！\n");
}

void showallmessage()
{
	int i = 0;
	printf("\t【南工智能聊天服务器】已登入服务器的QQ为:\n");
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

/*  系统菜单  */
void displayMenu()
{
	printf("\t************************************************\n");
	printf("\t*                                              *\n");
	printf("\t*    南工智能聊天服务器                        *\n");
	printf("\t*              V1.0                            *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*      1) 启动 (s)                             *\n");
	printf("\t*      2) 显示登录用户(u)                      *\n");
	printf("\t*      3) 退出(e)                              *\n");
	printf("\t*                                              *\n");
	printf("\t************************************************\n");
	printf("\n");
	printf("\t您的选择是:");

}

int main(int agrc, char* argv[])
{
	displayMenu();
	procMenu();
}
