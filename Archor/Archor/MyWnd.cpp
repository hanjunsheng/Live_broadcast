// MyWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyWnd.h"
#include "packdef.h"


// CMyWnd
int CMyWnd::is=0;
IMPLEMENT_DYNCREATE(CMyWnd, CFrameWnd)

CMyWnd::CMyWnd()
{
	is=1;
}

CMyWnd::~CMyWnd()
{
	is=0;
}


BEGIN_MESSAGE_MAP(CMyWnd, CFrameWnd)
	ON_MESSAGE(UM_NM_RECVMSG,&CMyWnd::OnRecvMsg)
END_MESSAGE_MAP()


// CMyWnd ��Ϣ�������

LRESULT CMyWnd::OnRecvMsg(WPARAM wparam,LPARAM lparam)
{
	this->m_pudp->RecvFrom(wparam,lparam);
	return 0;
}
