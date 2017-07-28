#pragma once
#include "UDPNet.h"

// CMyWnd 框架

class CMyWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMyWnd)
protected:
	CMyWnd();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyWnd();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRecvMsg(WPARAM wparam,LPARAM lparam);
public:
	void SetptrUDP(CUDPNet * pudp)
	{
		this->m_pudp = pudp;
	}
private:
	CUDPNet * m_pudp;


private:
	static int is;
public:
	static CMyWnd * GetDX()
	{
		if(is==0)
		{
			CMyWnd * pwnd = new CMyWnd;
			return pwnd;
		}
		else
		{
			return NULL;
		}
	}
	static void DestroyObject(CMyWnd * &p)
	{
		delete p;
		p = NULL;
	}
};


