// IPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Audience.h"
#include "IPDlg.h"
#include "afxdialogex.h"
#include "AudienceDlg.h"
#include "AbsIMediator.h"


// CIPDlg 对话框

IMPLEMENT_DYNAMIC(CIPDlg, CDialogEx)

CIPDlg::CIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIPDlg::IDD, pParent)
{

}

CIPDlg::~CIPDlg()
{
}

void CIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_serveripaddress);
}


BEGIN_MESSAGE_MAP(CIPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CIPDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CIPDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CIPDlg 消息处理程序


void CIPDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//确定按钮

	//设置服务器地址
	DWORD address;
	this->m_serveripaddress.GetAddress(address);
	address = ntohl(address);

	((CAudienceDlg *)GetParent())->GetIMediator()->SetServerAddr(address);

	//连接tcp
	if(false == ((CABsIMediator *)((CAudienceDlg *)GetParent())->GetIMediator())->m_tcp->Connect())
	{
		return ;
	}

	//更改对话框
	ShowWindow(SW_HIDE);
	((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg.ShowWindow(SW_SHOW);
	((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg.InitialDlg();
	((CAudienceDlg *)AfxGetMainWnd())->SetMainWnd(((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg);
	((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg.SetFocus();
}


void CIPDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//退出按钮

	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}


void CIPDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	this->OnBnClickedButton1();
}


void CIPDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
}
