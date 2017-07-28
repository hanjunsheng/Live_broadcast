// RegisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Archor.h"
#include "RegisDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "ArchorDlg.h"


// CRegisDlg �Ի���

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


// CRegisDlg ��Ϣ�������


void CRegisDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ע�ᰴť

	//�ж�������ȷ�������Ƿ���ͬ
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
		MessageBox(_T("����д��ȷ���û������룡"));
		return ;
	}

	if(strname.GetLength() > 10)
	{
		MessageBox(_T("�û����ĳ��ȿ�����10���ַ�֮�ڣ�"));
		return ;
	}
	if(strpass.GetLength() > 10)
	{
		MessageBox(_T("����ĳ��ȿ�����10���ַ�֮�ڣ�"));
		return ;
	}
	if(strsecondname.GetLength() > 10)
	{
		MessageBox(_T("�ǳƵĳ��ȿ�����10���ַ�֮�ڣ�"));
		return ;
	}


	if(strpass != strtpass)
	{
		MessageBox(_T("�����������벻��ͬ������������"));
		return ;
	}

	//---------����--------------------
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ذ�ť
	this->m_regisname.SetWindowText(_T(""));
	this->m_regispass.SetWindowText(_T(""));
	this->m_registwopass.SetWindowText(_T(""));

	this->ShowWindow(SW_HIDE);
}


void CRegisDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	this->OnBnClickedButton1();
}

