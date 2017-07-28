#pragma once
#include "INet.h"
#include "IMediator.h"

class CTCPNet : public CINet
{
public:
	CTCPNet(CIMediator * pme);
	~CTCPNet();
public:
	bool Initial();
	void UnInitial();
	static unsigned __stdcall ThreadRecv( void * lpvoid);
	bool Send(char * szbuf,int len);
	bool Connect();
	void RecvFrom(WPARAM wparam,LPARAM lparam);
	void SetServerAddress(long IP);
	bool ReceiveFile();
private:
	HANDLE m_hthread;
	bool m_bflagquit;
	SOCKET m_socket;
	CIMediator * m_pme;
	sockaddr_in m_serveraddress;
};