#pragma once
#include "afxwin.h"


// CRegisDlg �Ի���

class CRegisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisDlg)

public:
	CRegisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisDlg();

// �Ի�������
	enum { IDD = IDD_REGISTERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
