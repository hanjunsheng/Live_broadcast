#pragma once
#include "afxwin.h"
#include <Windows.h>
#include "GiftPicture.h"
#include <list>
#include "packdef.h"
using namespace std;

// CMainDlg 对话框

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	CListBox m_lstmsg;
	CEdit m_editmsg;
	CGiftPicture1 m_gif1;
	CGiftPicture2 m_gif2;
	CGiftPicture3 m_gif3;
	CGiftPicture4 m_gif4;
	CGiftPicture5 m_gif5;
	CGiftPicture6 m_gif6;
public:
	CString myname;
	CString archorsecond;
	list<TCHAR *> m_lstfavorite;
private:
	bool ChangeFont();
public:
	afx_msg void OnBnClickedButton6();
	CStatic m_picture;
	afx_msg void OnStnDblclickPicture();
	virtual void OnCancel();
public:
	list<STRU_DANMUINFO *> m_lstdanmu;
	COLORREF m_textcolor; 
	CRITICAL_SECTION m_csdm;
	void Move();
	void Show();
};
