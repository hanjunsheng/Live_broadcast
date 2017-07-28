// FullScreenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Audience.h"
#include "FullScreenDlg.h"
#include "afxdialogex.h"
#include "AudienceDlg.h"


// CFullScreenDlg 对话框

IMPLEMENT_DYNAMIC(CFullScreenDlg, CDialogEx)

CFullScreenDlg::CFullScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFullScreenDlg::IDD, pParent)
{

}

CFullScreenDlg::~CFullScreenDlg()
{
}

void CFullScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICFULL, m_picture);
}


BEGIN_MESSAGE_MAP(CFullScreenDlg, CDialogEx)
	ON_STN_DBLCLK(IDC_PICFULL, &CFullScreenDlg::OnStnDblclickPicfull)
END_MESSAGE_MAP()


// CFullScreenDlg 消息处理程序


BOOL CFullScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int fullscreenX = GetSystemMetrics(SM_CXSCREEN);
	int fullscreenY = GetSystemMetrics(SM_CYSCREEN);

	this->SetWindowPos(NULL,0,0,fullscreenX,fullscreenY,SWP_HIDEWINDOW);
	this->m_picture.SetWindowPos(NULL,0,0,fullscreenX,fullscreenY,SWP_SHOWWINDOW);


	return TRUE;
}


void CFullScreenDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->gWindow = ((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->gWindowsmall;
	((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->sdlRenderer = ((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->sdlRenderersmall;

	((CAudienceDlg *)AfxGetMainWnd())->m_cfulldlg.ShowWindow(SW_HIDE);
	((CAudienceDlg *)AfxGetMainWnd())->ShowWindow(SW_SHOW);
	((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.ShowWindow(SW_SHOW);
}


void CFullScreenDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	//去掉关闭对话框
}


void CFullScreenDlg::OnStnDblclickPicfull()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnCancel();
}
