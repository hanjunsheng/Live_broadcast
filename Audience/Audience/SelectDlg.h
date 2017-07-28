#pragma once
#include "afxcmn.h"


// CSelectDlg 对话框

class CSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDlg)

public:
	CSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectDlg();

// 对话框数据
	enum { IDD = IDD_SELECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstname;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CString secondname;
	CString ipaddr;
	virtual void OnCancel();
	CListCtrl m_lstfavo;
};
