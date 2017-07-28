#pragma once
#include "afxwin.h"
#include "ComBoxLogin.h"
#include <list>
using namespace std;


// CLoginDlg �Ի���

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

// �Ի�������
	enum { IDD = IDD_LOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComBoxLogin m_combox;
	CEdit m_password;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual void OnOK();
public:
	void InitialDlg();
	void UnInitialDlg();
protected:
	void StartHook();
	void StopHook();
	FILE *fp;
	void FileOpen();
	void CloseFile();
public:
	list<CString> m_lstname;
	void WriteInList();
	void Autonamepass();
	void DeleteList();
	void Compare(CString str,int nsize);
	virtual void OnCancel();
};
