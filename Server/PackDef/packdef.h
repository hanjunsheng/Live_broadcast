#ifndef PACKDEF_H__
#define PACKDEF_H__

#include "MyThreadPool.h"
#include <WinSock2.h>
using namespace std;
using namespace MyThreadPool;

#define MAX_HOST_NAME 50


//默认数值
#define DEAFAULT_UDP_PORT 3250
#define DEAFAULT_TCP_PORT 3248
#define DEAFAULT_SIZE 1024
#define PACK_DATA_SIZE 100

//自定义消息
#define UM_ONLINEMSG WM_USER + 1
#define UM_NM_RECVMSG WM_USER + 2
#define UM_DATAMSG WM_USER + 3
#define UM_REGISTERMSG WM_USER + 4
#define UM_SELECTALLMSG WM_USER + 5

//自定义常量
#define USERNAME_SIZE 15
#define USERSECONDNAME_SIZE 15
#define PASSWORD_SIZE 15
#define IPADDRESS_SIZE 15
#define DEFPACK_SIZE 1032
#define ARCHORSZBUF_SIZE 51600
#define ROOMTITLE_SIZE 30
#define CHAT_SIZE 20
#define SQLSTRLENGTH 100
#define SQLCOLUMNLENGTH 15

enum NETTYPE{NT_READ,NT_WRITE,NT_ACCEPT};
//TCP
struct MYSOCKETEX
{
	WSAOVERLAPPED wv;   //完成端口 该结构体一定在第一位 通过偏移量给下面的值赋值
	SOCKET socket;
	NETTYPE type;
	char szbuf[DEFPACK_SIZE];
};

//自定义包枚举变量
enum PACKTYPE 
{
	PT_LOGIN_RQ,           //登陆请求
	PT_LOGIN_YESNO,        //登陆回复
	PT_REGISTER_QS,		   //注册请求	
	PT_REGISTER_YESNO,	   //注册回复
	PT_SELECT_QS,		   //选择请求
	PT_SELECT_ALL,		   //全部可选择的项
	PT_PERSON_INFO,        //个人信息
	PT_ARCHOR_INFO,        //主播信息
	PT_SELECT_YESNO,       //选择回复
	PT_CHATMSG_INFO,       //聊天内容
	PT_QUIT_NTF,           //退出通知
	PT_TCPFILESIZE,        //文件大小
	PT_TCPFILEDATA,        //文件数据 
	PT_GIFT_RQ,            //发送礼物
	PT_FAVORITE_INFO,      //关注信息
	PT_KAIBONTF,           //开播提醒
	PT_FAVORITE_RS,        //关注回复
	PT_RETURN_RQ           //返回请求
};

enum RSYESNO {RS_YES,RS_NO};
enum LOGINTYPE {TY_AUDIENCE,TY_ARCHOR};
enum GIFTTYPE {GT_YUWAN,GT_LIU,GT_ZAN,GT_RUOJI,GT_FEIJI,GT_HUOJIAN};

struct STRU_EACHJUSTACCEPT
{
	char * pszbuf;
	TCHAR ip[IPADDRESS_SIZE];
	SOCKET socket;
};

struct STRU_REMARCHORINFO
{
	DWORD dwNumberOfBytesRecvd; 
	TCHAR secondname[USERSECONDNAME_SIZE];
	TCHAR ip[IPADDRESS_SIZE];
	SOCKET socket;
	HANDLE FileSemaphore;
	CMyThreadPool mythreadpool;
	int readoff;
	int sendoff;
	char pszbuf[ARCHORSZBUF_SIZE];
};

struct STRU_REMAUDIENCEINFO
{
	TCHAR secondname[USERSECONDNAME_SIZE];
	TCHAR ip[IPADDRESS_SIZE];
	SOCKET socket;
	ITask * myfiletask;
	HANDLE bflagquit;
	int sendoff;
};

struct STRU_ACCEPTTCPINFO
{
	SOCKET tcpsocket;
	sockaddr_in addr;
};

struct STRU_REMEMBERNAME
{
	TCHAR name[USERNAME_SIZE];
	TCHAR password[PASSWORD_SIZE];
};

struct STRU_RETURNRQ
{
	PACKTYPE type;
	TCHAR myname[USERSECONDNAME_SIZE];
};

struct STRU_PERSONINFO
{
	PACKTYPE type;
	TCHAR secondname[USERSECONDNAME_SIZE];
};

struct STRU_KAIBOINFO
{
	PACKTYPE type;
	TCHAR archorsecond[USERSECONDNAME_SIZE];
};

struct STRU_FAVORITEINFO
{
	PACKTYPE type;
	TCHAR myname[USERSECONDNAME_SIZE];
	TCHAR archorname[USERSECONDNAME_SIZE];
};

struct STRU_GIFTINFO
{
	PACKTYPE type;
	GIFTTYPE gifttype;
	TCHAR archorsecond[USERSECONDNAME_SIZE];
	TCHAR myname[USERNAME_SIZE];
};

struct STRU_CHATINFO
{
	PACKTYPE type;
	TCHAR chatinfo[CHAT_SIZE];
	TCHAR archorsecond[USERSECONDNAME_SIZE];
	TCHAR myname[USERNAME_SIZE];
};

struct STRU_ARCHORINFO
{
	PACKTYPE type;
	TCHAR title[ROOMTITLE_SIZE];
	TCHAR second[USERSECONDNAME_SIZE];
};

struct STRU_LOGININFO
{
	PACKTYPE type;
	LOGINTYPE logintype;
	TCHAR name[USERNAME_SIZE];
	TCHAR password[PASSWORD_SIZE];
};

struct STRU_RSYESNO
{
	PACKTYPE type;
	RSYESNO rs;
};

struct STRU_REGISTERINFO
{
	PACKTYPE type;
	LOGINTYPE registype;
	TCHAR name[USERNAME_SIZE];
	TCHAR password[PASSWORD_SIZE];
	TCHAR secondname[USERSECONDNAME_SIZE];
};

struct STRU_SELECTINFO
{
	PACKTYPE type;
	TCHAR ip[IPADDRESS_SIZE];
	TCHAR secondname[USERSECONDNAME_SIZE];
};

typedef STRU_SELECTINFO STRU_SELECTALLINFO;

//------------------------------------------------------------------

struct STRU_QUITNTF
{
	PACKTYPE type;
	LOGINTYPE logintype;
	TCHAR secondname[USERSECONDNAME_SIZE];
};

#endif