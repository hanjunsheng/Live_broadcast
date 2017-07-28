// ComBoxLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Archor.h"
#include "ComBoxLogin.h"


// CComBoxLogin

IMPLEMENT_DYNAMIC(CComBoxLogin, CComboBox)

CComBoxLogin::CComBoxLogin()
{
	str = _T("");
}

CComBoxLogin::~CComBoxLogin()
{
}


BEGIN_MESSAGE_MAP(CComBoxLogin, CComboBox)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &CComBoxLogin::OnCbnCloseup)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, &CComBoxLogin::OnCbnEditchange)
END_MESSAGE_MAP()



// CComBoxLogin ��Ϣ�������
void CComBoxLogin::Initial()
{
	CString start;
	start = _T("");
	AddString(start);
	AddString(start);
	AddString(start);
	AddString(start);
	AddString(start);
}

void CComBoxLogin::CompareComBox(CString str,int nsize)
{
	int size = this->GetCount() - 1;
	if(size < 0)
	{
		this->ShowDropDown(FALSE);
		return ;
	}

	CString temp;
	for(int i=this->GetCount()-1;i>=0;i--)
	{
		this->GetLBText(i,temp);
		if(0 != wcsncmp(temp.GetBuffer(),str.GetBuffer(),nsize))
		{
			this->DeleteString(i);
		}
	}

	size = this->GetCount();
	if(size == 0)
	{
		this->ShowDropDown(FALSE);
	}
}

void CComBoxLogin::DeleteCombox()
{
	int size = this->GetCount() - 1;
	if(size < 0)
	{
		this->ShowDropDown(FALSE);
		return ;
	}

	for(int i=this->GetCount()-1;i>=0;i--)
	{
		this->DeleteString(i);
	}

	this->ShowDropDown(FALSE);
}



void CComBoxLogin::OnCbnCloseup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int count = GetCount();
	if(count == 0)
		return ;

	int index = GetCurSel();
	if(CB_ERR == index)
	{
		SetWindowText(str);
	}
}


void CComBoxLogin::OnCbnEditchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->GetWindowText(str);
}
