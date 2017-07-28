#pragma once
#include "INet.h"
#include "IMediator.h"

class CMyWnd;

class CUDPNet : public CINet
{
public:
	CUDPNet(CIMediator * pme);
	~CUDPNet();
public:
	bool Initial();
	void UnInitial();
	bool Send(char * szbuf,int len);
	void CUDPNet::RecvFrom(WPARAM wparam,LPARAM lparam);
	void SetServerAddress(long IP);
private:
	bool m_bflagquit;
	SOCKET m_socket;
	CIMediator * m_pme;
	CMyWnd * m_mywnd;
	sockaddr_in m_serveraddress;
};