// SelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Audience.h"
#include "SelectDlg.h"
#include "afxdialogex.h"
#include "AudienceDlg.h"
#include "packdef.h"
#include "AbsIMediator.h"


// CSelectDlg 对话框

IMPLEMENT_DYNAMIC(CSelectDlg, CDialogEx)

CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectDlg::IDD, pParent)
{

}

CSelectDlg::~CSelectDlg()
{
}

void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstname);
	DDX_Control(pDX, IDC_LIST2, m_lstfavo);
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSelectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSelectDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSelectDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// CSelectDlg 消息处理程序


BOOL CSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	this->m_lstname.InsertColumn(0,_T("正在播放的主播"));
	this->m_lstname.InsertColumn(1,_T("IP地址"));
	CRect rect;
	this->m_lstname.GetWindowRect(&rect);
	this->m_lstname.SetColumnWidth(0,rect.Width() / 2);
	this->m_lstname.SetColumnWidth(1,rect.Width() / 2);

	this->m_lstname.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	this->m_lstfavo.InsertColumn(0,_T("关注的主播"));
	this->m_lstfavo.GetWindowRect(&rect);
	this->m_lstfavo.SetColumnWidth(0,rect.Width());

	this->m_lstfavo.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return FALSE;
}

void CSelectDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->OnBnClickedButton1();
}

void CSelectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//确定
	int index = this->m_lstname.GetSelectionMark();
	if(index == -1)
	{
		MessageBox(_T("请选择一项"));
		return ;
	}

	secondname = this->m_lstname.GetItemText(index,0);
	ipaddr = this->m_lstname.GetItemText(index,1);
	((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.archorsecond = secondname;
	STRU_SELECTINFO info;
	info.type = PT_SELECT_QS;
	wcscpy_s(info.secondname,15,((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.myname.GetBuffer());
	wcscpy_s(info.ip,15,ipaddr.GetBuffer());

	((CAudienceDlg *)AfxGetMainWnd())->GetIMediator()->SendData((char *)&info,sizeof(STRU_SELECTINFO));

	((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.GetDlgItem(IDC_NAME2)->SetWindowText(info.secondname);
	((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.GetDlgItem(IDC_IPADDR2)->SetWindowText(info.ip);
}


void CSelectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//退出
	((CAudienceDlg *)AfxGetMainWnd())->PostMessage(WM_CLOSE);
}


void CSelectDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	this->OnBnClickedButton1();

	*pResult = 0;
}


void CSelectDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
}
