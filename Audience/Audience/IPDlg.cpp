// IPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Audience.h"
#include "IPDlg.h"
#include "afxdialogex.h"
#include "AudienceDlg.h"
#include "AbsIMediator.h"


// CIPDlg �Ի���

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


// CIPDlg ��Ϣ�������


void CIPDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ȷ����ť

	//���÷�������ַ
	DWORD address;
	this->m_serveripaddress.GetAddress(address);
	address = ntohl(address);

	((CAudienceDlg *)GetParent())->GetIMediator()->SetServerAddr(address);

	//����tcp
	if(false == ((CABsIMediator *)((CAudienceDlg *)GetParent())->GetIMediator())->m_tcp->Connect())
	{
		return ;
	}

	//���ĶԻ���
	ShowWindow(SW_HIDE);
	((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg.ShowWindow(SW_SHOW);
	((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg.InitialDlg();
	((CAudienceDlg *)AfxGetMainWnd())->SetMainWnd(((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg);
	((CAudienceDlg *)AfxGetMainWnd())->m_clogindlg.SetFocus();
}


void CIPDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�˳���ť

	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}


void CIPDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	this->OnBnClickedButton1();
}


void CIPDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
}
