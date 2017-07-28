#include "stdafx.h"
#include "AbsIMediator.h"
#include "Archor.h"
#include "ArchorDlg.h"

CABsIMediator::CABsIMediator()
{
	m_udp = new CUDPNet(this);
	m_tcp = new CTCPNet(this);
	m_myffmpeg = new CMyffmpeg;
}

CABsIMediator::~CABsIMediator()
{
	if(m_udp)
	{
		delete m_udp;
		m_udp = NULL;
	}

	if(m_tcp)
	{
		delete m_tcp;
		m_tcp = NULL;
	}

	if(m_myffmpeg)
	{
		delete m_myffmpeg;
		m_myffmpeg = NULL;
	}
}

void CABsIMediator::SetServerAddr(long IP)
{
	this->m_udp->SetServerAddress(IP);
	this->m_tcp->SetServerAddress(IP);
}

bool CABsIMediator::Open(HWND hwnd)
{
	if(hwnd == NULL)
		return false;

	m_hwnd = hwnd;

	if(!m_udp->Initial())
	{
		return false;
	}

	if(!m_tcp->Initial())
	{
		return false;
	}

	return true;
}

void CABsIMediator::Close()
{
	//发送退出消息
	STRU_QUITNTF sqn;
	sqn.type = PT_QUIT_NTF;
	sqn.logintype = TY_ARCHOR;
	wcscpy_s(sqn.secondname,15,((CArchorDlg *)AfxGetMainWnd())->m_cmaindlg.myname);
	this->SendData((char *)&sqn,sizeof(STRU_QUITNTF));

	//退出
	m_udp->UnInitial();
	m_tcp->UnInitial();
	m_myffmpeg->Delete();
	CoInitialize(NULL);
}

bool CABsIMediator::SendData(char *szbuf,int len)
{
	if(!m_udp->Send(szbuf,len))
	{
		return false;
	}
	return true;
}

bool CABsIMediator::DealData(char *szbuf)
{
	int *ptype = (int *)szbuf;

	switch(*ptype)
	{
	case PT_LOGIN_YESNO:
		{
			STRU_RSYESNO * prs = (STRU_RSYESNO *)szbuf;
			SendMessage(m_hwnd,UM_ONLINEMSG,prs->rs,0);
		}
		break;
	case PT_REGISTER_YESNO:
		{
			STRU_RSYESNO * prs = (STRU_RSYESNO *)szbuf;
			SendMessage(m_hwnd,UM_REGISTERMSG,prs->rs,0);
		}
		break;
	case PT_PERSON_INFO:
		{
			SendMessage(m_hwnd,UM_PERSONINFOMSG,0,(LPARAM)szbuf);
		}
		break;
	case PT_CHATMSG_INFO:
		{
			SendMessage(m_hwnd,UM_CHATMSG,0,(LPARAM)szbuf);
		}
		break;
	case PT_ARCHOR_INFO:
		{
			SendMessage(m_hwnd,UM_ARCHORMSG,0,(LPARAM)szbuf);
		}
		break;
	case PT_GIFT_RQ:
		{
			SendMessage(m_hwnd,UM_GIFTMSG,0,(LPARAM)szbuf);
		}
		break;
	}
	return true;
}
