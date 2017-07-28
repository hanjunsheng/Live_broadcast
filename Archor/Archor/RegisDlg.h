#pragma once
#include "afxwin.h"


// CRegisDlg 对话框

class CRegisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisDlg)

public:
	CRegisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisDlg();

// 对话框数据
	enum { IDD = IDD_REGISTERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_regisname;
	CEdit m_regispass;
	CEdit m_registwopass;
	CEdit m_registypedef;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual void OnOK();
};
