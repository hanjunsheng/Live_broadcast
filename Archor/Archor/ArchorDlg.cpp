
// ArchorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Archor.h"
#include "ArchorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CArchorDlg 对话框


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


// CArchorDlg 消息处理程序
LRESULT CArchorDlg::OnLoginMsg(WPARAM wparam,LPARAM lparam)
{
	if(wparam == RS_YES)
	{
		//更改对话框
		this->m_clogindlg.ShowWindow(SW_HIDE);
		this->m_cmaindlg.ShowWindow(SW_SHOW);
		this->SetMainWnd(m_cmaindlg);
		this->m_cmaindlg.SetFocus();
	}
	else
	{
		MessageBox(_T("登陆错误,请尝试重新登陆"));
	}

	return 0;
}

LRESULT CArchorDlg::OnRegisterMsg(WPARAM wparam,LPARAM lparam)
{
	if(wparam == RS_YES)
	{
		//更改对话框
		this->m_cregisdlg.MessageBox(_T("注册成功"));
		this->m_cregisdlg.ShowWindow(SW_HIDE);
	}
	else
	{
		this->m_cregisdlg.MessageBox(_T("注册失败"));
	}
	return 0;
}

LRESULT CArchorDlg::OnPersonMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_PERSONINFO * sp = (STRU_PERSONINFO *)lparam;
	CString str;
	str += _T("欢迎：");
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

		//弹幕
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
			gift = _T("鱼丸");
			break;
		case GT_LIU:
			gift = _T("666");
			break;
		case GT_ZAN:
			gift = _T("赞");
			break;
		case GT_RUOJI:
			gift = _T("弱鸡");
			break;
		case GT_FEIJI:
			gift = _T("飞机");
			break;
		case GT_HUOJIAN:
			gift = _T("火箭");
			break;
		}

		CString str;
		str += sg->myname;
		str += _T("送给主播1个");
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

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//随机数种子
	srand((unsigned int)time(0));

	//输入IP对话框
	m_cipdlg.Create(IDD_IPDLG,this);
	m_cipdlg.ShowWindow(SW_SHOW);
	this->SetMainWnd(m_cipdlg);

	//登陆对话框
	m_clogindlg.Create(IDD_LOGINDLG,this);
	m_clogindlg.ShowWindow(SW_HIDE);

	//注册对话框
	m_cregisdlg.Create(IDD_REGISTERDLG,this);
	m_clogindlg.ShowWindow(SW_HIDE);

	//主界面对话框
	m_cmaindlg.Create(IDD_MAINDLG,this);
	m_cmaindlg.ShowWindow(SW_HIDE);

	//摄像头对话框
	m_ccameradlg.Create(IDD_CAMERADLG,this);
	m_ccameradlg.ShowWindow(SW_HIDE);

	//初始化
	if(false == this->m_absme->Open(this->m_hWnd))
	{
		MessageBox(_T("网络初始化失败！"));
		return FALSE;
	}

	this->m_bflagisinitial = true;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CArchorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

	// TODO: 在此处添加消息处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(this->m_cmaindlg.bflagisstart == true)
	{
		MessageBox(_T("请先关闭串流在退出"));
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

	 //TODO: 在此处添加消息处理程序代码
	if(this->m_bflagisinitial == true)
	{
		if(this->m_cmaindlg.camera == true)
		{
			this->m_ccameradlg.ShowWindow(SW_SHOW);
		}
	}
}
