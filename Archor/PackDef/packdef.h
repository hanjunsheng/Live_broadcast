#ifndef PACKDEF_H__
#define PACKDEF_H__

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
#define UM_PERSONINFOMSG WM_USER + 6
#define UM_SELECTRSMSG WM_USER + 7
#define UM_ARCHORMSG WM_USER + 8
#define UM_CHATMSG WM_USER + 9
#define UM_GIFTMSG WM_USER + 10

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
	PT_FAVORITE_RS         //关注回复
};

enum RSYESNO {RS_YES,RS_NO};
enum LOGINTYPE {TY_AUDIENCE,TY_ARCHOR};
enum GIFTTYPE {GT_YUWAN,GT_LIU,GT_ZAN,GT_RUOJI,GT_FEIJI,GT_HUOJIAN};

//弹幕信息
struct STRU_DANMUINFO
{
	CString str;
	CRect rect;
};

struct STRU_ACCEPTTCPINFO
{
	SOCKET tcpsocket;
	sockaddr_in addr;
};

struct STRU_REMEMBERNAME
{
	TCHAR name[15];
	TCHAR password[15];
};

//消息包
struct STRU_KAIBOINFO
{
	PACKTYPE type;
	TCHAR archorsecond[15];
};

struct STRU_GIFTINFO
{
	PACKTYPE type;
	GIFTTYPE gifttype;
	TCHAR archorsecond[15];
	TCHAR myname[15];
};

struct STRU_CHATINFO
{
	PACKTYPE type;
	TCHAR chatinfo[20];
	TCHAR archorsecond[15];
	TCHAR myname[15];
};

struct STRU_PERSONINFO
{
	PACKTYPE type;
	TCHAR secondname[15];
};

struct STRU_ARCHORINFO
{
	PACKTYPE type;
	TCHAR title[30];
	TCHAR second[15];
};

struct STRU_LOGININFO
{
	PACKTYPE type;
	LOGINTYPE logintype;
	TCHAR name[15];
	TCHAR password[15];
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
	TCHAR name[15];
	TCHAR password[15];
	TCHAR secondname[15];
};

struct STRU_SELECTINFO
{
	PACKTYPE type;
	TCHAR ip[15];
	TCHAR secondname[15];
};

typedef STRU_SELECTINFO STRU_SELECTALLINFO;

enum  FILETYPE{FT_FILESIZE,FT_FILEDATA};

struct STRU_DATAINFO
{
	FILETYPE type;
	int filesize;
	char szbuf[DEAFAULT_SIZE];
};
typedef STRU_DATAINFO STRU_DATASIZEINFO;

struct STRU_QUITNTF
{
	PACKTYPE type;
	LOGINTYPE logintype;
	TCHAR secondname[15];
};

#endif