// CameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Archor.h"
#include "CameraDlg.h"
#include "afxdialogex.h"


// CCameraDlg 对话框

IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraDlg::IDD, pParent)
{
	this->dwExStyle = 0;
	this->dwStyle = 0;
	this->m_bflagisnoframe = false;
}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_STN_DBLCLK(IDC_PICTURE, &CCameraDlg::OnStnDblclickPicture)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CCameraDlg 消息处理程序

void CCameraDlg::OnStnDblclickPicture()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->m_bflagisnoframe == false)
	{
		this->dwStyle = GetStyle();
		DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		dwNewStyle &= dwStyle;
		SetWindowLong(m_hWnd,GWL_STYLE,dwNewStyle);
		this->dwExStyle = GetExStyle();
		DWORD dwNewExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
		dwNewExStyle &= dwExStyle;
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwNewExStyle);
		SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
		this->m_bflagisnoframe = !this->m_bflagisnoframe;
	}
	else
	{
		SetWindowLong(m_hWnd,GWL_STYLE,dwStyle);
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle);
		SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

		this->m_bflagisnoframe = !this->m_bflagisnoframe;
	}
}

void CCameraDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	this->GetWindowRect(&rect);

	this->m_picture.SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
}
