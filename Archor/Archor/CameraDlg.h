#pragma once
#include "afxwin.h"


// CCameraDlg 对话框

class CCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCameraDlg();

// 对话框数据
	enum { IDD = IDD_CAMERADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picture;
public:
	DWORD dwStyle;
	DWORD dwExStyle;
	bool m_bflagisnoframe;
	afx_msg void OnStnDblclickPicture();
	afx_msg void OnPaint();
};
