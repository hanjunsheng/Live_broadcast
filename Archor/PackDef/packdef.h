#ifndef PACKDEF_H__
#define PACKDEF_H__

#define MAX_HOST_NAME 50

//Ĭ����ֵ
#define DEAFAULT_UDP_PORT 3250
#define DEAFAULT_TCP_PORT 3248
#define DEAFAULT_SIZE 1024
#define PACK_DATA_SIZE 100

//�Զ�����Ϣ
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

//�Զ����ö�ٱ���
enum PACKTYPE 
{
	PT_LOGIN_RQ,           //��½����
	PT_LOGIN_YESNO,        //��½�ظ�
	PT_REGISTER_QS,		   //ע������	
	PT_REGISTER_YESNO,	   //ע��ظ�
	PT_SELECT_QS,		   //ѡ������
	PT_SELECT_ALL,		   //ȫ����ѡ�����
	PT_PERSON_INFO,        //������Ϣ
	PT_ARCHOR_INFO,        //������Ϣ
	PT_SELECT_YESNO,       //ѡ��ظ�
	PT_CHATMSG_INFO,       //��������
	PT_QUIT_NTF,           //�˳�֪ͨ
	PT_TCPFILESIZE,        //�ļ���С
	PT_TCPFILEDATA,        //�ļ�����
	PT_GIFT_RQ,            //��������
	PT_FAVORITE_INFO,      //��ע��Ϣ
	PT_KAIBONTF,           //��������
	PT_FAVORITE_RS         //��ע�ظ�
};

enum RSYESNO {RS_YES,RS_NO};
enum LOGINTYPE {TY_AUDIENCE,TY_ARCHOR};
enum GIFTTYPE {GT_YUWAN,GT_LIU,GT_ZAN,GT_RUOJI,GT_FEIJI,GT_HUOJIAN};

//��Ļ��Ϣ
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

//��Ϣ��
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