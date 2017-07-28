// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Archor.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "ArchorDlg.h"
#include "packdef.h"


// CLoginDlg 对话框

//钩子
HHOOK hook;
LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam,LPARAM lParam);

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	this->fp = NULL;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_EDIT1, m_password);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CLoginDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//登陆
	this->UnInitialDlg();

	CString str;
	this->m_combox.GetWindowText(str);
	CString pas;
	this->m_password.GetWindowText(pas);

	if(str == _T("") || pas == _T(""))
	{
		MessageBox(_T("请输入正确的用户名密码"));
		return ;
	}

	//记住用户名和密码
	STRU_REMEMBERNAME remembername;
	ZeroMemory(remembername.name,sizeof(remembername.name));
	ZeroMemory(remembername.password,sizeof(remembername.password));
	if(((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck() == BST_CHECKED)
	{
		wcscpy_s(remembername.name,15,str);
	}
	if(((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck() == BST_CHECKED && ((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck() == BST_CHECKED)
	{
		wcscpy_s(remembername.password,15,pas);
	}
	FILE *floginauto = NULL;
	fopen_s(&floginauto,"../TempFile/autoname.txt","wb+");
	fwrite(&remembername,sizeof(STRU_REMEMBERNAME),1,floginauto);
	fclose(floginauto);
	floginauto = NULL;
	
	//写入文件
	fopen_s(&fp,"../TempFile/username.txt","r");
	this->WriteInList();
	if(fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}

	list<CString>::iterator ite = this->m_lstname.begin();

	bool iswrite = true;
	while(ite != this->m_lstname.end())
	{
		if(0 == wcscmp(str.GetBuffer(),(*ite).GetBuffer()))
		{
			iswrite = false;
			break;
		}
		++ite;
	}

	if(iswrite == true)
	{
		TCHAR name[25] = {0};
		wcscpy_s(name,25,str.GetBuffer());
		fopen_s(&this->fp,"../TempFile/username.txt","a+");
	
		fwrite(&name,50,1,this->fp);
		fclose(this->fp);
		this->fp = NULL;
	}
	//发送登陆数据
	STRU_LOGININFO sl;
	wcscpy_s(sl.name,15,str.GetBuffer());
	wcscpy_s(sl.password,15,pas.GetBuffer()); 
	sl.type = PT_LOGIN_RQ;
	sl.logintype = TY_ARCHOR;
	((CArchorDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&sl,sizeof(STRU_LOGININFO));

	//删掉链表
	this->DeleteList();
}

void CLoginDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//注册
	((CArchorDlg *)AfxGetMainWnd())->m_cregisdlg.ShowWindow(SW_SHOW);
}

void CLoginDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//退出
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->OnBnClickedButton1();
}

void CLoginDlg::Autonamepass()
{
	STRU_REMEMBERNAME rename;
	FILE *fp = NULL;
	fopen_s(&fp,"../TempFile/autoname.txt","rb+");
	if(fp == NULL)
	{
		return ;
	}
	fread(&rename,sizeof(STRU_REMEMBERNAME),1,fp);
	fclose(fp);
	fp = NULL;

	if(0 != wcscmp(rename.name,_T("")))
	{
		this->m_combox.SetWindowText(rename.name);
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED);
	}

	if(0 != wcscmp(rename.password,_T("")))
	{
		this->m_password.SetWindowText(rename.password);
		((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(BST_CHECKED);
	}
}

void CLoginDlg::InitialDlg()
{
	this->StartHook();
	this->FileOpen();
	this->WriteInList();
	this->m_combox.Initial();
	this->Autonamepass();
}

void CLoginDlg::UnInitialDlg()
{
	//关闭钩子 关闭文件
	this->StopHook();
	this->CloseFile();
}

void CLoginDlg::StartHook()
{
	hook = SetWindowsHookEx(WH_KEYBOARD,&KeyboardProc,NULL,GetCurrentThreadId());
}

void CLoginDlg::StopHook()
{
	UnhookWindowsHookEx(hook);
}

void CLoginDlg::FileOpen()
{
	fopen_s(&this->fp,"../TempFile/username.txt","r");
}

void CLoginDlg::CloseFile()
{
	if(this->fp == NULL)
		return ;

	fclose(this->fp);
	this->fp = NULL;
}

void CLoginDlg::WriteInList()
{
	//打开文件失败  则返回
	if(this->fp == NULL)
		return ;

	//读入链表
	TCHAR name[25] = {0};
	while(1)
	{
		int nes = fread(name,50,1,fp);
		if(nes <= 0)
		{
			break;
		}
		CString str;
		str = name;
		this->m_lstname.push_back(str);
	}
}

void CLoginDlg::DeleteList()
{
	this->m_lstname.clear();
}

LRESULT CALLBACK KeyboardProc(int code,WPARAM wParam,LPARAM lParam)
{
	if(GetFocus() == ((CArchorDlg *)AfxGetMainWnd())->m_clogindlg.m_password.m_hWnd)
		return CallNextHookEx(hook,code,wParam,lParam);
	if(GetFocus() == ((CArchorDlg *)AfxGetMainWnd())->m_cregisdlg.m_regisname.m_hWnd)
		return CallNextHookEx(hook,code,wParam,lParam);
	if(GetFocus() == ((CArchorDlg *)AfxGetMainWnd())->m_cregisdlg.m_regispass.m_hWnd)
		return CallNextHookEx(hook,code,wParam,lParam);
	if(GetFocus() == ((CArchorDlg *)AfxGetMainWnd())->m_cregisdlg.m_registwopass.m_hWnd)
		return CallNextHookEx(hook,code,wParam,lParam);

	char strtemp[2] = {0};
	if((!(lParam & 0x80000000)) && ((wParam >= 0x30 && wParam <= 0x39) || (wParam >= 0x41 && wParam <= 0x5A)))
	{
		if(wParam >= 0x30 && wParam <= 0x39)
		{
			_itoa_s(wParam - 48, &strtemp[0],2,10);
		}
		else if(wParam >= 0x41 && wParam <= 0x5A)
		{
			BYTE ks[256];
			GetKeyboardState(ks);
			WORD w;
			UINT scan;
			scan = 0;
			ToAscii(wParam,scan,ks,&w,0);
			strtemp[0] = char(w);
		}

		CString str;
		((CArchorDlg *)AfxGetMainWnd())->m_clogindlg.m_combox.GetWindowText(str);
		int nsize = str.GetLength() + 1;
		str += strtemp;
		
		if(nsize == 1)
		{	
			((CArchorDlg *)AfxGetMainWnd())->m_clogindlg.m_combox.DeleteCombox();
			((CArchorDlg *)AfxGetMainWnd())->m_clogindlg.Compare(str,nsize);
		}
		else
		{
			((CArchorDlg *)AfxGetMainWnd())->m_clogindlg.m_combox.CompareComBox(str,nsize);
		}
	}

	return CallNextHookEx(hook,code,wParam,lParam);
}

void CLoginDlg::Compare(CString str,int nsize)
{
	list<CString>::iterator ite = this->m_lstname.begin();
	while(ite != this->m_lstname.end())
	{
		if(0 == wcsncmp((*ite).GetBuffer(),str.GetBuffer(),nsize))
		{
			this->m_combox.AddString(*ite);
			this->m_combox.ShowDropDown(TRUE);
		}

		++ite;
	}
}


void CLoginDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
}
