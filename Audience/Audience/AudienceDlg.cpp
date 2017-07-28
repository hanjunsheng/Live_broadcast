
// AudienceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Audience.h"
#include "AudienceDlg.h"
#include "afxdialogex.h"
#include "packdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAudienceDlg �Ի���

CAudienceDlg::CAudienceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAudienceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->m_absme = new CABsIMediator;
}

void CAudienceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAudienceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_ONLINEMSG,&CAudienceDlg::OnLoginMsg)
	ON_MESSAGE(UM_REGISTERMSG,&CAudienceDlg::OnRegisterMsg)
	ON_MESSAGE(UM_SELECTALLMSG,&CAudienceDlg::OnSelectAllMsg)
	ON_MESSAGE(UM_PERSONINFOMSG,&CAudienceDlg::OnPersonMsg)
	ON_MESSAGE(UM_SELECTRSMSG,&CAudienceDlg::OnSelectRSMsg)
	ON_MESSAGE(UM_ARCHORMSG,&CAudienceDlg::OnArchorMsg)
	ON_MESSAGE(UM_CHATMSG,&CAudienceDlg::OnChatMsg)
	ON_MESSAGE(UM_QUITNTF,&CAudienceDlg::OnQuitNTF)
	ON_MESSAGE(UM_GIFTMSG,&CAudienceDlg::OnGiftMsg)
	ON_MESSAGE(UM_FAVORITERSSG,&CAudienceDlg::OnFavoriteMsg)
	ON_MESSAGE(UM_ALLFAVORITESG,&CAudienceDlg::OnAllFavoriteMsg)
	ON_MESSAGE(UM_KAIBONTFMSG,&CAudienceDlg::OnKaiBoNTFMsg)
END_MESSAGE_MAP()


// CAudienceDlg ��Ϣ�������

LRESULT CAudienceDlg::OnPersonMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_PERSONINFO * sp = (STRU_PERSONINFO *)lparam;
	CString str;
	str += _T("��ӭ��");
	str += sp->secondname;

	this->m_cselectdlg.GetDlgItem(IDC_WELCOME)->SetWindowText(str);
	this->m_cmaindlg.GetDlgItem(IDC_USERNAME)->SetWindowText(sp->secondname);
	this->m_cmaindlg.myname = sp->secondname;

	return 0;
}

LRESULT CAudienceDlg::OnLoginMsg(WPARAM wparam,LPARAM lparam)
{
	if(wparam == RS_YES)
	{
		//���ĶԻ���
		this->m_clogindlg.ShowWindow(SW_HIDE);
		this->m_cselectdlg.ShowWindow(SW_SHOW);
		this->SetMainWnd(m_cselectdlg);
		this->m_cselectdlg.SetFocus();
	}
	else
	{
		MessageBox(_T("��½����,�볢�����µ�½"));
	}

	return 0;
}

LRESULT CAudienceDlg::OnRegisterMsg(WPARAM wparam,LPARAM lparam)
{
	if(wparam == RS_YES)
	{
		//���ĶԻ���
		this->m_cregisdlg.MessageBox(_T("ע��ɹ�"));
		this->m_cregisdlg.ShowWindow(SW_HIDE);
	}
	else
	{
		this->m_cregisdlg.MessageBox(_T("ע��ʧ��"));
	}
	return 0;
}

LRESULT CAudienceDlg::OnSelectAllMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_SELECTALLINFO * psai = (STRU_SELECTALLINFO *)lparam;
	
	int index = this->m_cselectdlg.m_lstname.GetItemCount();
	if(index > 0)
	{
		for(size_t i=index-1;i>=0;i--)
		{
			CString str;
			str = this->m_cselectdlg.m_lstname.GetItemText(i,0);
			if(0 == wcscmp(str,psai->secondname))
			{
				return 0;
			}
		}
	}
	this->m_cselectdlg.m_lstname.InsertItem(index,psai->secondname);
	this->m_cselectdlg.m_lstname.SetItemText(index,1,psai->ip);

	return 0;
}

LRESULT CAudienceDlg::OnArchorMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_ARCHORINFO * sa = (STRU_ARCHORINFO *)lparam;

	if(0 == wcscmp(this->m_cmaindlg.archorsecond,sa->second))
	{
		this->m_cmaindlg.GetDlgItem(IDC_TITLE)->SetWindowText(sa->title);
	}

	return 0;
}

LRESULT CAudienceDlg::OnSelectRSMsg(WPARAM wparam,LPARAM lparam)
{
	if(wparam == RS_YES)
	{
		//ת�����Ի���
		this->m_cselectdlg.ShowWindow(SW_HIDE);
		this->m_cmaindlg.m_lstmsg.ResetContent();
		this->m_cmaindlg.ShowWindow(SW_SHOW);
		this->SetMainWnd(((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg);
		this->m_cmaindlg.SetFocus();
	}
	else
	{
		MessageBox(_T("��ȡʧ���볢�����»�ȡ����"));
	}
	return 0;
}

LRESULT CAudienceDlg::OnChatMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_CHATINFO * sc = (STRU_CHATINFO *)lparam;

	if(0 == wcscmp(sc->archorsecond,this->m_cmaindlg.archorsecond))
	{
		CString str;
		str += sc->myname;
		str += _T(":");
		str += sc->chatinfo;
		this->m_cmaindlg.m_lstmsg.AddString(str);

		//��Ļ
		CRect dcrect;
		this->m_cmaindlg.m_picture.GetWindowRect(&dcrect);
		TEXTMETRIC tm;
		CClientDC dc(&this->m_cmaindlg.m_picture);
		dc.GetTextMetrics(&tm);

		STRU_DANMUINFO * pdm = new STRU_DANMUINFO;
		pdm->str = sc->chatinfo;
		pdm->rect.right = dcrect.Width();
		pdm->rect.left = dcrect.Width();
		pdm->rect.top = rand()%(dcrect.Height()/5);
		pdm->rect.bottom = pdm->rect.top + tm.tmHeight;

		EnterCriticalSection(&this->m_cmaindlg.m_csdm);
		this->m_cmaindlg.m_lstdanmu.push_back(pdm);
		LeaveCriticalSection(&this->m_cmaindlg.m_csdm);
	}

	return 0;
}

LRESULT CAudienceDlg::OnGiftMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_GIFTINFO * sg = (STRU_GIFTINFO *)lparam;

	CString gift;
	if(0 == wcscmp(sg->archorsecond,this->m_cmaindlg.archorsecond))
	{
		switch(sg->gifttype)
		{
		case GT_YUWAN:
			gift = _T("����");
			break;
		case GT_LIU:
			gift = _T("666");
			break;
		case GT_ZAN:
			gift = _T("��");
			break;
		case GT_RUOJI:
			gift = _T("����");
			break;
		case GT_FEIJI:
			gift = _T("�ɻ�");
			break;
		case GT_HUOJIAN:
			gift = _T("���");
			break;
		}

		CString str;
		str += sg->myname;
		str += _T("�͸�����1��");
		str += gift;
		this->m_cmaindlg.m_lstmsg.AddString(str);
	}
	return 0;
}

LRESULT CAudienceDlg::OnQuitNTF(WPARAM wparam,LPARAM lparam)
{
	STRU_QUITNTF * sqn = (STRU_QUITNTF *)lparam;
	if(0 == wcscmp(this->m_cmaindlg.archorsecond,sqn->secondname))
	{
		//�ر�����ֱ��
		MessageBox(_T("�����ѹر�ֱ�����뷵��"));
	}

	int index = this->m_cselectdlg.m_lstname.GetItemCount();
	for(size_t i=index-1;i>=0;i--)
	{
		CString str;
		str = this->m_cselectdlg.m_lstname.GetItemText(i,0);
		if(0 == wcscmp(str,sqn->secondname))
		{
			this->m_cselectdlg.m_lstname.DeleteItem(i);
			break;
		}
	}

	return 0;
}

LRESULT CAudienceDlg::OnFavoriteMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_RSYESNO * sr = (STRU_RSYESNO *)lparam;
	if(sr->rs == RS_YES)
	{
		this->m_cmaindlg.GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		MessageBox(_T("��ע�ɹ�"));
	}
	else
	{
		MessageBox(_T("��עʧ��"));
	}
	return 0;
}

LRESULT CAudienceDlg::OnAllFavoriteMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_FAVORITEINFO * sf = (STRU_FAVORITEINFO *)lparam;

	if(0 == wcscpy_s(sf->myname,this->m_cmaindlg.myname.GetBuffer()))
	{
		this->m_cselectdlg.m_lstfavo.InsertItem(0,sf->archorname);

		TCHAR * str = new TCHAR[15];
		wcscpy_s(str,15,sf->archorname);	
		this->m_cmaindlg.m_lstfavorite.push_back(str);
	}

	return 0;
}

LRESULT CAudienceDlg::OnKaiBoNTFMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_KAIBOINFO * sk = (STRU_KAIBOINFO *)lparam;

	list<TCHAR *>::iterator ite = this->m_cmaindlg.m_lstfavorite.begin();
	while(ite != this->m_cmaindlg.m_lstfavorite.end())
	{
		if(0 == wcscmp(sk->archorname,(*ite)))
		{
			CString str;
			str += _T("����ע������:");
			str += sk->archorname;
			str += _T("��ʼֱ���ˣ�");
			MessageBox(str);
			break;
		}
		++ite;
	}

	return 0;
}

BOOL CAudienceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//���������
	srand((unsigned int)time(0));

	//����IP�Ի���
	m_cipdlg.Create(IDD_IPDLG,this);
	m_cipdlg.ShowWindow(SW_SHOW);
	this->SetMainWnd(m_cipdlg);

	//��½�Ի���
	m_clogindlg.Create(IDD_LOGINDLG,this);
	m_clogindlg.ShowWindow(SW_HIDE);

	//ע��Ի���
	m_cregisdlg.Create(IDD_REGISTERDLG,this);
	m_clogindlg.ShowWindow(SW_HIDE);

	//ѡ��Ի���
	m_cselectdlg.Create(IDD_SELECTDLG,this);
	m_cselectdlg.ShowWindow(SW_HIDE);

	//������Ի���
	m_cmaindlg.Create(IDD_MAINDLG,this);
	m_cmaindlg.ShowWindow(SW_HIDE);

	//ȫ���Ի���
	m_cfulldlg.Create(IDD_FULLSCREEN,this);
	m_cfulldlg.ShowWindow(SW_HIDE);

	//��ʼ��
	if(false == this->m_absme->Open(this->m_hWnd))
	{
		MessageBox(_T("�����ʼ��ʧ�ܣ�"));
		return FALSE;
	}


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAudienceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAudienceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAudienceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAudienceDlg::SetMainWnd(CDialogEx &dlg)
{
	CRect rect;
	dlg.GetWindowRect(&rect);

	int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.Width() + 16)) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.Height() + 38)) / 2;
	int wide = rect.Width() + 16;
	int height = rect.Height() + 38;

	this->SetWindowPos(NULL,x,y,wide,height,SWP_SHOWWINDOW);
}



void CAudienceDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if(m_absme)
	{
		this->m_absme->Close();
		delete m_absme;
		m_absme = NULL;
	}

	list<TCHAR *>::iterator ite = this->m_cmaindlg.m_lstfavorite.begin();
	while(ite != this->m_cmaindlg.m_lstfavorite.end())
	{
		delete[] *ite;
		*ite = NULL;
		++ite;
	}
}

CIMediator * CAudienceDlg::GetIMediator()
{
	return this->m_absme;
}
