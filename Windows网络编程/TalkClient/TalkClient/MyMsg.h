#pragma once
/************************************************************
*  �ļ�����MyMsg.h
*  ������  �������Ϳͻ��˹��õ�ͷ�ļ�����Ϣ����
*************************************************************/
#ifndef _MYMSG_H_
#define _MYMSG_H_

enum {
	LOGIN_MSG,           //��¼
	LOGIN_MSG_RES,       //��¼��Ӧ
	QUERY_FQQ,           //��ѯ��������QQ
	QUERY_FQQ_RES,       //��ѯ��������QQ����Ӧ
	TALK_MSG,             //����
	REQUEST_ALL_FRD,      //��������qq������Ϣ
	APPLY_ALL_FRD        //Ӧ������qq������Ϣ
};

struct LoginMsg {
	unsigned char id;  //��Ϣ���
	char qq[6];        //��¼ID��
};

struct LoginMsgResponse {
	unsigned char id;  //��Ϣ���
	unsigned char isOK;
	char reason[100];    //���ʧ�ܣ����ڴ�д��ʧ��ԭ��
};

struct FriendQqMsg {
	unsigned char id;  //��Ϣ���
	char qq[6];
};

struct FriendQqMsgResponse {
	unsigned char id;  //��Ϣ���
	unsigned char isOK;
};

struct TalkMsg {
	unsigned char id;  //��Ϣ���
	char qq[6];
	char fqq[6];
	char info[200];
};

struct RequestAllFriend {
	unsigned char id;   //��Ϣ���
};

struct AllFriendMsg {
	unsigned char id;  //��Ϣ���
	char qq[6];
};

#endif