#pragma once
#include "afxwin.h"
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
//控件
public:
	CListBox m_lstmsg;
	CComboBox m_combox;
	CButton m_checkeddestop;
	CButton m_checkedcamera;
	CButton m_checkedaudio;
	CButton m_checkedmicro;
	CEdit m_editroomname;	
	CEdit m_editmsg;
//消息处理函数和重写的虚函数
public:
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton9();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
//控制bool变量
public:
	bool desk;
	bool camera;
	bool audio;
	bool microphone;
	bool m_bflagissetvalue;
	bool bflagisstart;
//其他变量
public:
	CString myname;
	list<STRU_DANMUINFO *> m_lstdanmu;
	CStatic m_picture;
	COLORREF m_textcolor; 
	CRITICAL_SECTION m_csdm;
//弹幕移动
public:
	void Move();
	void Show();
	afx_msg void OnBnClickedButton7();
	CEdit m_devicename;
};
