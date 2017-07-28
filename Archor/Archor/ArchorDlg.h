
// ArchorDlg.h : ͷ�ļ�
//

#pragma once

#include "IPDlg.h"
#include "IMediator.h"
#include "AbsIMediator.h"
#include "LoginDlg.h"
#include "RegisDlg.h"
#include "MainDlg.h"
#include "CameraDlg.h"

// CArchorDlg �Ի���
class CArchorDlg : public CDialogEx
{
// ����
public:
	CArchorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ARCHOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
//�н���
private:
	CIMediator * m_absme;

// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
//����
public:
	void SetMainWnd(CDialogEx &dlg);
	CIMediator * GetIMediator();
//�Ի���
public:
	CIPDlg m_cipdlg;
	CLoginDlg m_clogindlg;
	CRegisDlg m_cregisdlg;
	CMainDlg m_cmaindlg;
	CCameraDlg m_ccameradlg;
//�Զ�����Ϣ
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
