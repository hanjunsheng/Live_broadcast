#pragma once
#include "afxwin.h"


// CFullScreenDlg 对话框

class CFullScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFullScreenDlg)

public:
	CFullScreenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFullScreenDlg();

// 对话框数据
	enum { IDD = IDD_FULLSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picture;
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnStnDblclickPicfull();
};
