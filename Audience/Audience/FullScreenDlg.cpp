// FullScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Audience.h"
#include "FullScreenDlg.h"
#include "afxdialogex.h"
#include "AudienceDlg.h"


// CFullScreenDlg �Ի���

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


// CFullScreenDlg ��Ϣ�������


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
	// TODO: �ڴ����ר�ô����/����û���

	((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->gWindow = ((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->gWindowsmall;
	((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->sdlRenderer = ((CABsIMediator *)((CAudienceDlg *)AfxGetMainWnd())->GetIMediator())->m_mydecoed->sdlRenderersmall;

	((CAudienceDlg *)AfxGetMainWnd())->m_cfulldlg.ShowWindow(SW_HIDE);
	((CAudienceDlg *)AfxGetMainWnd())->ShowWindow(SW_SHOW);
	((CAudienceDlg *)AfxGetMainWnd())->m_cmaindlg.ShowWindow(SW_SHOW);
}


void CFullScreenDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	//ȥ���رնԻ���
}


void CFullScreenDlg::OnStnDblclickPicfull()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnCancel();
}
