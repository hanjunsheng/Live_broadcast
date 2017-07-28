#pragma once


// CComBoxLogin

class CComBoxLogin : public CComboBox
{
	DECLARE_DYNAMIC(CComBoxLogin)

public:
	CComBoxLogin();
	virtual ~CComBoxLogin();
public:
	void CompareComBox(CString str,int nsize);
	void DeleteCombox();
	CString str;
	void Initial();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnCloseup();
	afx_msg void OnCbnEditchange();
};


