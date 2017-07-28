#pragma once
#include "INet.h"
#include "IMediator.h"

class CUDPNet : public CINet
{
public:
	CUDPNet(CIMediator * pme);
	~CUDPNet();
public:
	bool Initial();
	void UnInitial();
	bool Send(sockaddr_in *addr,char * szbuf,int len);
	static unsigned __stdcall ThreadRecv( void * lpvoid);
private:
	bool m_bflagquit;
	SOCKET m_socket;
	CIMediator * m_pme;
	HANDLE m_hthread;
};