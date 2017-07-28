
// ArchorDlg.h : 头文件
//

#pragma once

#include "IPDlg.h"
#include "IMediator.h"
#include "AbsIMediator.h"
#include "LoginDlg.h"
#include "RegisDlg.h"
#include "MainDlg.h"
#include "CameraDlg.h"

// CArchorDlg 对话框
class CArchorDlg : public CDialogEx
{
// 构造
public:
	CArchorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ARCHOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
//中介者
private:
	CIMediator * m_absme;

// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
//功能
public:
	void SetMainWnd(CDialogEx &dlg);
	CIMediator * GetIMediator();
//对话框
public:
	CIPDlg m_cipdlg;
	CLoginDlg m_clogindlg;
	CRegisDlg m_cregisdlg;
	CMainDlg m_cmaindlg;
	CCameraDlg m_ccameradlg;
//自定义消息
public:
	afx_msg LRESULT OnLoginMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnRegisterMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnPersonMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnChatMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnGiftMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnArchorRoomNameMsg(WPARAM wparam,LPARAM lparam);
public:
	bool m_bflagisinitial;
};
