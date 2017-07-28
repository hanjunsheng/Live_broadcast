// GiftPicture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Audience.h"
#include "GiftPicture.h"
#include "MainDlg.h"
#include "AudienceDlg.h"
#include "packdef.h"


// CGiftPicture

//��̬��������
IMPLEMENT_DYNAMIC(CGiftPicture1, CStatic)
IMPLEMENT_DYNAMIC(CGiftPicture2, CStatic)
IMPLEMENT_DYNAMIC(CGiftPicture3, CStatic)
IMPLEMENT_DYNAMIC(CGiftPicture4, CStatic)
IMPLEMENT_DYNAMIC(CGiftPicture5, CStatic)
IMPLEMENT_DYNAMIC(CGiftPicture6, CStatic)


//picture1----------------------------------------------------------------------------------------------
CGiftPicture1::CGiftPicture1()
{
	this->hbitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP10));
	this->hbitmappre = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP9));
	m_bMouseTrack = true;
}

CGiftPicture1::~CGiftPicture1()
{
	if(this->hbitmap != NULL)
	{
		::DeleteObject(this->hbitmap);
		this->hbitmap  = NULL;
	}
	if(this->hbitmappre != NULL)
	{
		::DeleteObject(this->hbitmappre);
		this->hbitmappre  = NULL;
	}
}

BEGIN_MESSAGE_MAP(CGiftPicture1, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_CONTROL_REFLECT(STN_CLICKED, &CGiftPicture1::OnStnClicked)
END_MESSAGE_MAP()

// CGiftPicture1 ��Ϣ�������

void CGiftPicture1::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	this->SetBitmap(this->hbitmap);

	if (m_bMouseTrack)     // ������ ׷�٣��� 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;// ָ��Ҫ ׷�� �Ĵ��� 
		csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧�� 

		m_bMouseTrack = FALSE ;   // ���Ѿ� ׷�� ����ֹͣ ׷�� 
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CGiftPicture1::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMouseTrack = true;
	this->SetBitmap(this->hbitmappre);

	CStatic::OnMouseLeave();
}

void CGiftPicture1::OnStnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STRU_GIFTINFO sg;
	sg.type = PT_GIFT_RQ;
	sg.gifttype = GT_YUWAN;
	wcscpy_s(sg.myname,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.myname.GetBuffer());
	wcscpy_s(sg.archorsecond,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.archorsecond.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sg,sizeof(STRU_GIFTINFO));
}


//picture2----------------------------------------------------------------------------------------------
CGiftPicture2::CGiftPicture2()
{
	this->hbitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2));
	this->hbitmappre = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1));
	m_bMouseTrack = true;
}

CGiftPicture2::~CGiftPicture2()
{
	if(this->hbitmap != NULL)
	{
		::DeleteObject(this->hbitmap);
		this->hbitmap  = NULL;
	}
	if(this->hbitmappre != NULL)
	{
		::DeleteObject(this->hbitmappre);
		this->hbitmappre  = NULL;
	}
}

BEGIN_MESSAGE_MAP(CGiftPicture2, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_CONTROL_REFLECT(STN_CLICKED, &CGiftPicture2::OnStnClicked)
END_MESSAGE_MAP()

// CGiftPicture2 ��Ϣ�������

void CGiftPicture2::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	this->SetBitmap(this->hbitmap);

	if (m_bMouseTrack)     // ������ ׷�٣��� 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;// ָ��Ҫ ׷�� �Ĵ��� 
		csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧�� 

		m_bMouseTrack=FALSE ;   // ���Ѿ� ׷�� ����ֹͣ ׷�� 
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CGiftPicture2::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMouseTrack = true;
	this->SetBitmap(this->hbitmappre);

	CStatic::OnMouseLeave();
}

void CGiftPicture2::OnStnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STRU_GIFTINFO sg;
	sg.type = PT_GIFT_RQ;
	sg.gifttype = GT_LIU;
	wcscpy_s(sg.myname,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.myname.GetBuffer());
	wcscpy_s(sg.archorsecond,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.archorsecond.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sg,sizeof(STRU_GIFTINFO));
}


//picture3----------------------------------------------------------------------------------------------
CGiftPicture3::CGiftPicture3()
{
	this->hbitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP12));
	this->hbitmappre = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP11));
	m_bMouseTrack = true;
}

CGiftPicture3::~CGiftPicture3()
{
	if(this->hbitmap != NULL)
	{
		::DeleteObject(this->hbitmap);
		this->hbitmap  = NULL;
	}
	if(this->hbitmappre != NULL)
	{
		::DeleteObject(this->hbitmappre);
		this->hbitmappre  = NULL;
	}
}

BEGIN_MESSAGE_MAP(CGiftPicture3, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_CONTROL_REFLECT(STN_CLICKED, &CGiftPicture3::OnStnClicked)
END_MESSAGE_MAP()

// CGiftPicture3 ��Ϣ�������

void CGiftPicture3::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	this->SetBitmap(this->hbitmap);

	if (m_bMouseTrack)     // ������ ׷�٣��� 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;// ָ��Ҫ ׷�� �Ĵ��� 
		csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧�� 

		m_bMouseTrack=FALSE ;   // ���Ѿ� ׷�� ����ֹͣ ׷�� 
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CGiftPicture3::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMouseTrack = true;
	this->SetBitmap(this->hbitmappre);

	CStatic::OnMouseLeave();
}

void CGiftPicture3::OnStnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STRU_GIFTINFO sg;
	sg.type = PT_GIFT_RQ;
	sg.gifttype = GT_ZAN;
	wcscpy_s(sg.myname,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.myname.GetBuffer());
	wcscpy_s(sg.archorsecond,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.archorsecond.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sg,sizeof(STRU_GIFTINFO));
}


//picture4----------------------------------------------------------------------------------------------
CGiftPicture4::CGiftPicture4()
{
	this->hbitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP8));
	this->hbitmappre = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP7));
	m_bMouseTrack = true;
}

CGiftPicture4::~CGiftPicture4()
{
	if(this->hbitmap != NULL)
	{
		::DeleteObject(this->hbitmap);
		this->hbitmap  = NULL;
	}
	if(this->hbitmappre != NULL)
	{
		::DeleteObject(this->hbitmappre);
		this->hbitmappre  = NULL;
	}
}

BEGIN_MESSAGE_MAP(CGiftPicture4, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_CONTROL_REFLECT(STN_CLICKED, &CGiftPicture4::OnStnClicked)
END_MESSAGE_MAP()

// CGiftPicture4 ��Ϣ�������

void CGiftPicture4::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	this->SetBitmap(this->hbitmap);

	if (m_bMouseTrack)     // ������ ׷�٣��� 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;// ָ��Ҫ ׷�� �Ĵ��� 
		csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧�� 

		m_bMouseTrack=FALSE ;   // ���Ѿ� ׷�� ����ֹͣ ׷�� 
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CGiftPicture4::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMouseTrack = true;
	this->SetBitmap(this->hbitmappre);

	CStatic::OnMouseLeave();
}

void CGiftPicture4::OnStnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STRU_GIFTINFO sg;
	sg.type = PT_GIFT_RQ;
	sg.gifttype = GT_RUOJI;
	wcscpy_s(sg.myname,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.myname.GetBuffer());
	wcscpy_s(sg.archorsecond,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.archorsecond.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sg,sizeof(STRU_GIFTINFO));
}


//picture5----------------------------------------------------------------------------------------------
CGiftPicture5::CGiftPicture5()
{
	this->hbitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP4));
	this->hbitmappre = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP3));
	m_bMouseTrack = true;
}

CGiftPicture5::~CGiftPicture5()
{
	if(this->hbitmap != NULL)
	{
		::DeleteObject(this->hbitmap);
		this->hbitmap  = NULL;
	}
	if(this->hbitmappre != NULL)
	{
		::DeleteObject(this->hbitmappre);
		this->hbitmappre  = NULL;
	}
}

BEGIN_MESSAGE_MAP(CGiftPicture5, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_CONTROL_REFLECT(STN_CLICKED, &CGiftPicture5::OnStnClicked)
END_MESSAGE_MAP()

// CGiftPicture5 ��Ϣ�������

void CGiftPicture5::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	this->SetBitmap(this->hbitmap);

	if (m_bMouseTrack)     // ������ ׷�٣��� 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;// ָ��Ҫ ׷�� �Ĵ��� 
		csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧�� 

		m_bMouseTrack=FALSE ;   // ���Ѿ� ׷�� ����ֹͣ ׷�� 
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CGiftPicture5::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMouseTrack = true;
	this->SetBitmap(this->hbitmappre);

	CStatic::OnMouseLeave();
}

void CGiftPicture5::OnStnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STRU_GIFTINFO sg;
	sg.type = PT_GIFT_RQ;
	sg.gifttype = GT_FEIJI;
	wcscpy_s(sg.myname,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.myname.GetBuffer());
	wcscpy_s(sg.archorsecond,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.archorsecond.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sg,sizeof(STRU_GIFTINFO));
}


//picture6----------------------------------------------------------------------------------------------
CGiftPicture6::CGiftPicture6()
{
	this->hbitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP6));
	this->hbitmappre = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP5));
	m_bMouseTrack = true;
}

CGiftPicture6::~CGiftPicture6()
{
	if(this->hbitmap != NULL)
	{
		::DeleteObject(this->hbitmap);
		this->hbitmap  = NULL;
	}
	if(this->hbitmappre != NULL)
	{
		::DeleteObject(this->hbitmappre);
		this->hbitmappre  = NULL;
	}
}

BEGIN_MESSAGE_MAP(CGiftPicture6, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_CONTROL_REFLECT(STN_CLICKED, &CGiftPicture6::OnStnClicked)
END_MESSAGE_MAP()

// CGiftPicture6 ��Ϣ�������

void CGiftPicture6::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	this->SetBitmap(this->hbitmap);

	if (m_bMouseTrack)     // ������ ׷�٣��� 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd ;// ָ��Ҫ ׷�� �Ĵ��� 
		csTME.dwHoverTime = 10;  // ����ڰ�ť��ͣ������ 10ms ������Ϊ״̬Ϊ HOVER
		::_TrackMouseEvent (&csTME); // ���� Windows �� WM_MOUSELEAVE �� WM_MOUSEHOVER �¼�֧�� 

		m_bMouseTrack=FALSE ;   // ���Ѿ� ׷�� ����ֹͣ ׷�� 
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CGiftPicture6::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bMouseTrack = true;
	this->SetBitmap(this->hbitmappre);

	CStatic::OnMouseLeave();
}

void CGiftPicture6::OnStnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STRU_GIFTINFO sg;
	sg.type = PT_GIFT_RQ;
	sg.gifttype = GT_HUOJIAN;
	wcscpy_s(sg.myname,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.myname.GetBuffer());
	wcscpy_s(sg.archorsecond,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.archorsecond.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sg,sizeof(STRU_GIFTINFO));
}

