#pragma once
#include "afxcmn.h"


// CIPDlg 对话框

class CIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIPDlg)

public:
	CIPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIPDlg();

// 对话框数据
	enum { IDD = IDD_IPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_serveripaddress;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual void OnOK();
	virtual void OnCancel();
};
