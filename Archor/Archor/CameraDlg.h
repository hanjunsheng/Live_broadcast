#pragma once
#include "afxwin.h"


// CCameraDlg �Ի���

class CCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraDlg();

// �Ի�������
	enum { IDD = IDD_CAMERADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
