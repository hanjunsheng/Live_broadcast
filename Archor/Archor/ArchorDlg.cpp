
// ArchorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Archor.h"
#include "ArchorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CArchorDlg �Ի���


CArchorDlg::CArchorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CArchorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->m_absme = new CABsIMediator;
	m_bflagisinitial = false;
}

void CArchorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CArchorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_ONLINEMSG,&CArchorDlg::OnLoginMsg)
	ON_MESSAGE(UM_REGISTERMSG,&CArchorDlg::OnRegisterMsg)
	ON_MESSAGE(UM_PERSONINFOMSG,&CArchorDlg::OnPersonMsg)
	ON_MESSAGE(UM_CHATMSG,&CArchorDlg::OnChatMsg)
	ON_MESSAGE(UM_GIFTMSG,&CArchorDlg::OnGiftMsg)
	ON_MESSAGE(UM_ARCHORMSG,&CArchorDlg::OnArchorRoomNameMsg)
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CArchorDlg ��Ϣ�������
LRESULT CArchorDlg::OnLoginMsg(WPARAM wparam,LPARAM lparam)
{
	if(wparam == RS_YES)
	{
		//���ĶԻ���
		this->m_clogindlg.ShowWindow(SW_HIDE);
		this->m_cmaindlg.ShowWindow(SW_SHOW);
		this->SetMainWnd(m_cmaindlg);
		this->m_cmaindlg.SetFocus();
	}
	else
	{
		MessageBox(_T("��½����,�볢�����µ�½"));
	}

	return 0;
}

LRESULT CArchorDlg::OnRegisterMsg(WPARAM wparam,LPARAM lparam)
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

LRESULT CArchorDlg::OnPersonMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_PERSONINFO * sp = (STRU_PERSONINFO *)lparam;
	CString str;
	str += _T("��ӭ��");
	str += sp->secondname;

	this->m_cmaindlg.GetDlgItem(IDC_USERNAME)->SetWindowText(sp->secondname);
	this->m_cmaindlg.myname = sp->secondname;

	return 0;
}

LRESULT CArchorDlg::OnChatMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_CHATINFO * sc = (STRU_CHATINFO *)lparam;

	if(0 == wcscmp(sc->archorsecond,this->m_cmaindlg.myname))
	{
		CString str;
		str += sc->myname;
		str += _T(":");
		str += sc->chatinfo;
		this->m_cmaindlg.m_lstmsg.AddString(str);

		//��Ļ
		CRect dcrect;
		this->m_cmaindlg.m_picture.GetWindowRect(&dcrect);
		ClientToScreen(&dcrect);
		TEXTMETRIC tm;
		CClientDC dc(&this->m_cmaindlg.m_picture);
		dc.GetTextMetrics(&tm);

		STRU_DANMUINFO * pdm = new STRU_DANMUINFO;
		pdm->str = sc->chatinfo;
		pdm->rect.right = dcrect.right;
		pdm->rect.left = dcrect.right;
		pdm->rect.top = rand()%(dcrect.Height()/5) + dcrect.top;
		pdm->rect.bottom = pdm->rect.top + tm.tmHeight;

		this->m_cmaindlg.m_lstdanmu.push_back(pdm);
	}

	return 0;
}

LRESULT CArchorDlg::OnGiftMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_GIFTINFO * sg = (STRU_GIFTINFO *)lparam;

	CString gift;
	if(0 == wcscmp(sg->archorsecond,this->m_cmaindlg.myname))
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

LRESULT CArchorDlg::OnArchorRoomNameMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_ARCHORINFO *sa = (STRU_ARCHORINFO *)lparam;

	this->m_cmaindlg.m_editroomname.SetWindowText(sa->title);

	return 0;
}

BOOL CArchorDlg::OnInitDialog()
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

	//������Ի���
	m_cmaindlg.Create(IDD_MAINDLG,this);
	m_cmaindlg.ShowWindow(SW_HIDE);

	//����ͷ�Ի���
	m_ccameradlg.Create(IDD_CAMERADLG,this);
	m_ccameradlg.ShowWindow(SW_HIDE);

	//��ʼ��
	if(false == this->m_absme->Open(this->m_hWnd))
	{
		MessageBox(_T("�����ʼ��ʧ�ܣ�"));
		return FALSE;
	}

	this->m_bflagisinitial = true;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CArchorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CArchorDlg::OnPaint()
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
HCURSOR CArchorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CArchorDlg::SetMainWnd(CDialogEx &dlg)
{
	CRect rect;
	dlg.GetWindowRect(&rect);

	int x = (GetSystemMetrics(SM_CXSCREEN) - (rect.Width() + 16)) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - (rect.Height() + 38)) / 2;
	int wide = rect.Width() + 16;
	int height = rect.Height() + 38;

	this->SetWindowPos(NULL,x,y,wide,height,SWP_SHOWWINDOW);
}

void CArchorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if(m_absme)
	{
		this->m_absme->Close();
		delete m_absme;
		m_absme = NULL;
	}	
}

CIMediator * CArchorDlg::GetIMediator()
{
	return this->m_absme;
}


void CArchorDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(this->m_cmaindlg.bflagisstart == true)
	{
		MessageBox(_T("���ȹرմ������˳�"));
		return ;
	}

	this->m_bflagisinitial = false;

	if(this->m_cmaindlg.camera == true)
	{
		this->m_ccameradlg.ShowWindow(SW_HIDE);
	}

	CDialogEx::OnClose();
}


void CArchorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	 //TODO: �ڴ˴������Ϣ����������
	if(this->m_bflagisinitial == true)
	{
		if(this->m_cmaindlg.camera == true)
		{
			this->m_ccameradlg.ShowWindow(SW_SHOW);
		}
	}
}
