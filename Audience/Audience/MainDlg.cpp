// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Audience.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "AudienceDlg.h"


// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	m_textcolor = RGB(255,255,255);
	InitializeCriticalSection(&m_csdm);
}

CMainDlg::~CMainDlg()
{
	DeleteCriticalSection(&m_csdm);

	list<STRU_DANMUINFO *>::iterator ite = this->m_lstdanmu.begin();
	while(ite != this->m_lstdanmu.end())
	{
		delete *ite;
		*ite = NULL;
		++ite;
	}
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstmsg);
	DDX_Control(pDX, IDC_EDIT1, m_editmsg);
	DDX_Control(pDX, IDC_GIFT1, m_gif1);
	DDX_Control(pDX, IDC_GIFT2, m_gif2);
	DDX_Control(pDX, IDC_GIFT3, m_gif3);
	DDX_Control(pDX, IDC_GIFT4, m_gif4);
	DDX_Control(pDX, IDC_GIFT5, m_gif5);
	DDX_Control(pDX, IDC_GIFT6, m_gif6);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON5, &CMainDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CMainDlg::OnBnClickedButton6)
	ON_STN_DBLCLK(IDC_PICTURE, &CMainDlg::OnStnDblclickPicture)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//改变标题字体
	if(false == this->ChangeFont())
	{
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

bool CMainDlg::ChangeFont()
{
	CFont font;
	LOGFONT lf;  //逻辑字体结构
	::ZeroMemory(&lf,sizeof(lf));
	lf.lfHeight = 40;
	lf.lfWidth= 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_BOLD;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut= FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	font.CreateFontIndirect(&lf);
	this->GetDlgItem(IDC_TITLE)->SetFont(&font);

	return true;
}

void CMainDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->OnBnClickedButton1();
	return ;
}


void CMainDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//退出按钮
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}


void CMainDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//发送按钮
	CString str;
	this->m_editmsg.GetWindowText(str);
	if(str == _T(""))
	{
		MessageBox(_T("请输入内容"));
		return ;
	}

	STRU_CHATINFO sc;
	sc.type = PT_CHATMSG_INFO;
	wcscpy_s(sc.chatinfo,20,str);
	wcscpy_s(sc.archorsecond,15,this->archorsecond);
	wcscpy_s(sc.myname,15,this->myname.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sc,sizeof(STRU_CHATINFO));

	this->m_editmsg.SetWindowText(_T(""));
}


void CMainDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//全屏按钮
	
	((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->gWindow = ((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->gWindowfull;
	((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->sdlRenderer = ((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->sdlRendererfull;

	((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.ShowWindow(SW_HIDE);
	((CAudienceDlg *)AfxGetMainWnd())->ShowWindow(SW_HIDE);
	((CAudienceDlg *)AfxGetMainWnd())->m_cfulldlg.ShowWindow(SW_SHOW);
}


void CMainDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//关注按钮
	STRU_FAVORITEINFO sf;
	sf.type = PT_FAVORITE_INFO;
	wcscpy_s(sf.archorname,15,this->archorsecond);
	wcscpy_s(sf.myname,15,this->myname);

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sf,sizeof(STRU_FAVORITEINFO));
}

void CMainDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	//返回按钮

	STRU_RETURNRQ sr;
	sr.type = PT_RETURN_RQ;
	wcscpy_s(sr.myname,15,this->myname);
	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sr,sizeof(STRU_RETURNRQ));

	this->ShowWindow(SW_HIDE);
	((CAudienceDlg *)theApp.m_pMainWnd)->m_cselectdlg.ShowWindow(SW_SHOW);
	((CAudienceDlg *)theApp.m_pMainWnd)->SetMainWnd(((CAudienceDlg *)theApp.m_pMainWnd)->m_cselectdlg);
}

void CMainDlg::OnStnDblclickPicture()
{
	// TODO: 在此添加控件通知处理程序代码
	//双击picture ctrl

	this->OnBnClickedButton4();
}

void CMainDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
}

void CMainDlg::Move()
{
	EnterCriticalSection(&m_csdm);
	list<STRU_DANMUINFO *>::iterator ite = this->m_lstdanmu.begin();
	while(ite != this->m_lstdanmu.end())
	{
		(*ite)->rect.left -= 8;
		if((*ite)->rect.left <= -100)
		{
			delete *ite;
			*ite = NULL;
			ite = this->m_lstdanmu.erase(ite);
		}
		else
		{
			++ite;
		}
	}
	LeaveCriticalSection(&m_csdm);
}

void CMainDlg::Show()
{
	CClientDC dc(&m_picture);
	dc.SetTextColor(this->m_textcolor);
	dc.SetBkMode(TRANSPARENT);
	
	EnterCriticalSection(&m_csdm);
	list<STRU_DANMUINFO *>::iterator ite = this->m_lstdanmu.begin();
	while(ite != this->m_lstdanmu.end())
	{
		dc.DrawText((*ite)->str,(*ite)->rect,DT_LEFT);
		++ite;
	}
	LeaveCriticalSection(&m_csdm);
}

