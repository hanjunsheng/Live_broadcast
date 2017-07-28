// RegisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Archor.h"
#include "RegisDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "ArchorDlg.h"


// CRegisDlg 对话框

IMPLEMENT_DYNAMIC(CRegisDlg, CDialogEx)

CRegisDlg::CRegisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisDlg::IDD, pParent)
{

}

CRegisDlg::~CRegisDlg()
{
}

void CRegisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_regisname);
	DDX_Control(pDX, IDC_EDIT2, m_regispass);
	DDX_Control(pDX, IDC_EDIT3, m_registwopass);
	DDX_Control(pDX, IDC_EDIT4, m_registypedef);
}


BEGIN_MESSAGE_MAP(CRegisDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegisDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRegisDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CRegisDlg 消息处理程序


void CRegisDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//注册按钮

	//判断密码与确认密码是否相同
	CString strname;
	CString strpass;
	CString strtpass;
	CString strsecondname;

	this->m_regisname.GetWindowText(strname);
	this->m_regispass.GetWindowText(strpass);
	this->m_registwopass.GetWindowText(strtpass);
	this->m_registypedef.GetWindowText(strsecondname);

	if(strname == _T("") || strpass == _T("") || strtpass == _T("") || strsecondname == _T(""))
	{
		MessageBox(_T("请填写正确的用户名密码！"));
		return ;
	}

	if(strname.GetLength() > 10)
	{
		MessageBox(_T("用户名的长度控制在10个字符之内！"));
		return ;
	}
	if(strpass.GetLength() > 10)
	{
		MessageBox(_T("密码的长度控制在10个字符之内！"));
		return ;
	}
	if(strsecondname.GetLength() > 10)
	{
		MessageBox(_T("昵称的长度控制在10个字符之内！"));
		return ;
	}


	if(strpass != strtpass)
	{
		MessageBox(_T("两次输入密码不相同，请重新输入"));
		return ;
	}

	//---------发送--------------------
	STRU_REGISTERINFO info;
	info.type = PT_REGISTER_QS;
	info.registype = TY_ARCHOR;
	wcscpy_s(info.name,15,strname);
	wcscpy_s(info.password,15,strpass);
	wcscpy_s(info.secondname,15,strsecondname);

	((CArchorDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&info,sizeof(STRU_REGISTERINFO));
}


void CRegisDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//返回按钮
	this->m_regisname.SetWindowText(_T(""));
	this->m_regispass.SetWindowText(_T(""));
	this->m_registwopass.SetWindowText(_T(""));

	this->ShowWindow(SW_HIDE);
}


void CRegisDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->OnBnClickedButton1();
}

