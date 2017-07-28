#pragma once 
#include "IMediator.h"
#include "UDPNet.h"
#include "TCPNet.h"
#include <list>
#include "MyDao.h"
using namespace std;

class CABsIMediator : public CIMediator
{
public:
	CABsIMediator();
	~CABsIMediator();
//重写的
public:
	bool Open();
	void Close();
	bool SendData(sockaddr_in * addr,char *szbuf,int len);
	bool DealData(sockaddr_in * addr,char *szbuf);
	bool SendTCPData(char *szbuf,int len);
	bool DealTCPData(MYSOCKETEX * pmysock,char *szbuf);
	void Run();
//成员
public:
	CUDPNet * m_udp;
	CTCPNet * m_tcp;
	CMyDao m_ado;
private:
	void ShowHostIP();
//UDP处理函数
private:
	void DealLoginRQ(sockaddr_in *addr,char *szbuf);
	void DealRegisterRQ(sockaddr_in *addr,char *szbuf);
	void DealSelectRQ(sockaddr_in *addr,char *szbuf);
	void DealChatMSG(char *szbuf);
	void DealGiftMSG(char *szbuf);
	void DealKaiBoNTF(char *szbuf);
	void DealQuitNTF(char *szbuf);
	void DealFavoriteMSG(sockaddr_in *addr,char * szbuf);
	void DealArchorInfo(char *szbuf);
	bool SearchSQL(TCHAR * name,TCHAR *password,TCHAR *second);
	bool RegisSearchSQL(LOGINTYPE type,TCHAR * name,TCHAR *password,TCHAR *secondname);
	bool SelectSearchSQL(TCHAR * second,TCHAR *title);
	bool SerachFavoriteSQL(TCHAR * myname,TCHAR * archorname);
	bool SerachAllFavoriteSQL(TCHAR * myname,list<TCHAR *>& strvalue);
	void SetTitle(TCHAR * second,TCHAR *title);
	void DealReturnMSG(char * szbuf);
//TCP处理函数
public:
	void DealTCPaccept(sockaddr_in *addr,SOCKET sock,char * ps);
//主播观众的链表
public:
	list<STRU_EACHJUSTACCEPT *> m_lstjustaccpet;
	list<STRU_REMARCHORINFO *> m_lstahchor;
	list<STRU_REMAUDIENCEINFO *> m_lstaudience;
};
