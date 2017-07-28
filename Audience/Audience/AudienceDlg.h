
// AudienceDlg.h : ͷ�ļ�


#pragma once

#include "IPDlg.h"
#include "IMediator.h"
#include "AbsIMediator.h"
#include "LoginDlg.h"
#include "RegisDlg.h"
#include "SelectDlg.h"
#include "MainDlg.h"
#include "FullScreenDlg.h"

// CAudienceDlg �Ի���
class CAudienceDlg : public CDialogEx
{
// ����
public:
	CAudienceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUDIENCE_DIALOG };

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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
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
	CSelectDlg m_cselectdlg;
	CMainDlg m_cmaindlg;
	CFullScreenDlg m_cfulldlg;
//�Զ�����Ϣ
public:
	afx_msg LRESULT OnLoginMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnRegisterMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSelectAllMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnPersonMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnArchorMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnSelectRSMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnChatMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnQuitNTF(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnGiftMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnFavoriteMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnAllFavoriteMsg(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnKaiBoNTFMsg(WPARAM wparam,LPARAM lparam);
};
