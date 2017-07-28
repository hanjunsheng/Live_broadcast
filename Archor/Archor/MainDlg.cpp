// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Archor.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "ArchorDlg.h"
#include "AbsIMediator.h"
#include "ArchorDlg.h"


// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	desk = false;
	camera = false;
	audio = false;
	microphone = false;
	m_bflagissetvalue = false;
	bflagisstart = false;

	m_textcolor = RGB(255,255,255);
	InitializeCriticalSection(&m_csdm);
}

CMainDlg::~CMainDlg()
{
	DeleteCriticalSection(&m_csdm);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstmsg);
	DDX_Control(pDX, IDC_COMBO2, m_combox);
	DDX_Control(pDX, IDC_CHECK4, m_checkeddestop);
	DDX_Control(pDX, IDC_CHECK5, m_checkedcamera);
	DDX_Control(pDX, IDC_CHECK6, m_checkedaudio);
	DDX_Control(pDX, IDC_CHECK7, m_checkedmicro);
	DDX_Control(pDX, IDC_EDIT2, m_editroomname);
	DDX_Control(pDX, IDC_EDIT1, m_editmsg);
	DDX_Control(pDX, IDC_PICTRUE, m_picture);
	DDX_Control(pDX, IDC_EDIT3, m_devicename);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON8, &CMainDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CMainDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMainDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CMainDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON9, &CMainDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON7, &CMainDlg::OnBnClickedButton7)
END_MESSAGE_MAP()

// CMainDlg 消息处理程序

void CMainDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	this->OnBnClickedButton4();
}


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->m_combox.AddString(_T("1000000"));
	this->m_combox.AddString(_T("1800000"));
	this->m_combox.AddString(_T("2500000"));
	this->m_combox.AddString(_T("3500000"));
	this->m_combox.AddString(_T("5000000"));
	this->m_combox.AddString(_T("6000000"));
	this->m_combox.AddString(_T("7000000"));
	this->m_combox.AddString(_T("8500000"));
	this->m_combox.SelectString(-1,_T("1000000"));

	return TRUE;  

}

void CMainDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	//设置参数按钮
	if(this->bflagisstart == true)
	{
		MessageBox(_T("请先停止串流！"));
		return ;
	}
	
	if(((CButton *)GetDlgItem(IDC_CHECK4))->GetCheck() == BST_CHECKED)
	{
		desk = true;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK5))->GetCheck() == BST_CHECKED)
	{
		camera = true;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK6))->GetCheck() == BST_CHECKED)
	{
		audio = true;
	}

	if(((CButton *)GetDlgItem(IDC_CHECK7))->GetCheck() == BST_CHECKED)
	{
		microphone = true;
	}

	//获取码率
	long malv = 0;
	CString str;
	this->m_combox.GetWindowText(str);
	malv = _ttoi(str);

	//获取设备名称
	CString cameraname;
	this->m_devicename.GetWindowText(cameraname);
	((CABsIMediator *)((CArchorDlg *)GetParent())->GetIMediator())->m_myffmpeg->SetCameraDeviceName(cameraname);

	((CABsIMediator *)((CArchorDlg *)GetParent())->GetIMediator())->m_myffmpeg->Factory(desk,camera,audio,microphone);

	if(desk == true)
	{
		((CABsIMediator *)((CArchorDlg *)GetParent())->GetIMediator())->m_myffmpeg->m_collectdesktop->myencode.Setmalv(malv);
	}

	if(camera == true)
	{
		((CArchorDlg *)GetParent())->m_ccameradlg.ShowWindow(SW_SHOW);
	}

	this->m_bflagissetvalue = true;

	MessageBox(_T("设置成功!"));
}

void CMainDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//开始按钮
	if(this->m_bflagissetvalue == false)
	{
		MessageBox(_T("请设置参数！"));
		return ;
	}

	if(this->bflagisstart == true)
	{
		MessageBox(_T("已经开始推送，操作无效"));
		return ;
	}

	this->bflagisstart = true;

	((CABsIMediator *)((CArchorDlg *)GetParent())->GetIMediator())->m_myffmpeg->SetStart(desk,camera,audio,microphone);
}


void CMainDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	//停止按钮
	if(this->bflagisstart == false)
	{
		MessageBox(_T("未开始推送，操作无效"));
		return ;
	}

	this->bflagisstart = false;
	((CABsIMediator *)((CArchorDlg *)GetParent())->GetIMediator())->m_myffmpeg->SetStop(desk,camera,audio,microphone);
}


void CMainDlg::OnBnClickedButton4()
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
	wcscpy_s(sc.archorsecond,15,this->myname);
	wcscpy_s(sc.myname,15,this->myname);

	((CArchorDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sc,sizeof(STRU_CHATINFO));

	this->m_editmsg.SetWindowText(_T(""));
}


void CMainDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//清空按钮
	this->m_lstmsg.ResetContent();
}


void CMainDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//退出按钮
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}


void CMainDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	//设置房间标题按钮

	CString str;
	this->m_editroomname.GetWindowText(str);
	if(str == _T(""))
	{
		MessageBox(_T("请输入房间标题！"));
		return ;
	}

	STRU_ARCHORINFO sa;
	sa.type = PT_ARCHOR_INFO;
	wcscpy_s(sa.title,30,str);
	wcscpy_s(sa.second,15,this->myname);
	if(true == ((CArchorDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sa,sizeof(STRU_ARCHORINFO)))
	{
		MessageBox(_T("房间标题设置成功"));
	}
	else
	{
		MessageBox(_T("房间标题设置失败"));
	}
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
		(*ite)->rect.left -= 10;
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

void CMainDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	//开播提醒按钮

	STRU_KAIBOINFO sk;
	sk.type = PT_KAIBONTF;
	wcscpy_s(sk.archorsecond,15,this->myname);

	((CArchorDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sk,sizeof(STRU_KAIBOINFO));
}
