#pragma once
#include "afxcmn.h"


// CSelectDlg �Ի���

class CSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDlg)

public:
	CSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectDlg();

// �Ի�������
	enum { IDD = IDD_SELECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
