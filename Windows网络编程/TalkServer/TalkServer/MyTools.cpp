/************************************************************
*  �ļ�����MyTools.cpp
*  ������  ���õ��ļ������ߺ�������
*************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "MyTools.h"

extern int flag;

void printMsg(const char* msg) {
	if (flag)
		printf("\t���Ϲ������������桿%s", msg);
	else
		printf("\t���Ϲ��ͻ��˺����桿%s", msg);
}

void printInt(int value) {
	if (flag)
		printf("\t���Ϲ������������桿%d", value);
	else
		printf("\t���Ϲ��ͻ��˺����桿%d", value);
}

void printMsgExit(const char* msg) {
	printMsg(msg);
	exit(0);
}

void printIntExit(int value) {
	printInt(value);
	exit(0);
}