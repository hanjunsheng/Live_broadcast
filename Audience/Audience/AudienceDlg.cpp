
// AudienceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Audience.h"
#include "AudienceDlg.h"
#include "afxdialogex.h"
#include "packdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAudienceDlg 对话框

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


// CAudienceDlg 消息处理程序

LRESULT CAudienceDlg::OnPersonMsg(WPARAM wparam,LPARAM lparam)
{
	STRU_PERSONINFO * sp = (STRU_PERSONINFO *)lparam;
	CString str;
	str += _T("欢迎：");
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
		//更改对话框
		this->m_clogindlg.ShowWindow(SW_HIDE);
		this->m_cselectdlg.ShowWindow(SW_SHOW);
		this->SetMainWnd(m_cselectdlg);
		this->m_cselectdlg.SetFocus();
	}
	else
	{
		MessageBox(_T("登陆错误,请尝试重新登陆"));
	}

	return 0;
}

LRESULT CAudienceDlg::OnRegisterMsg(WPARAM wparam,LPARAM lparam)
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
		//转换主对话框
		this->m_cselectdlg.ShowWindow(SW_HIDE);
		this->m_cmaindlg.m_lstmsg.ResetContent();
		this->m_cmaindlg.ShowWindow(SW_SHOW);
		this->SetMainWnd(((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg);
		this->m_cmaindlg.SetFocus();
	}
	else
	{
		MessageBox(_T("获取失败请尝试重新获取数据"));
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

		//弹幕
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

LRESULT CAudienceDlg::OnQuitNTF(WPARAM wparam,LPARAM lparam)
{
	STRU_QUITNTF * sqn = (STRU_QUITNTF *)lparam;
	if(0 == wcscmp(this->m_cmaindlg.archorsecond,sqn->secondname))
	{
		//关闭正在直播
		MessageBox(_T("主播已关闭直播，请返回"));
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
		MessageBox(_T("关注成功"));
	}
	else
	{
		MessageBox(_T("关注失败"));
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
			str += _T("您关注的主播:");
			str += sk->archorname;
			str += _T("开始直播了！");
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

	//选择对话框
	m_cselectdlg.Create(IDD_SELECTDLG,this);
	m_cselectdlg.ShowWindow(SW_HIDE);

	//主界面对话框
	m_cmaindlg.Create(IDD_MAINDLG,this);
	m_cmaindlg.ShowWindow(SW_HIDE);

	//全屏对话框
	m_cfulldlg.Create(IDD_FULLSCREEN,this);
	m_cfulldlg.ShowWindow(SW_HIDE);

	//初始化
	if(false == this->m_absme->Open(this->m_hWnd))
	{
		MessageBox(_T("网络初始化失败！"));
		return FALSE;
	}


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAudienceDlg::OnPaint()
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

	// TODO: 在此处添加消息处理程序代码
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
