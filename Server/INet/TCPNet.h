#pragma once
#include "INet.h"
#include "IMediator.h"
#include <list>
#include <iostream>
#include <Mswsock.h>
#include "packdef.h"
#include "MyThreadPool.h"

#pragma comment(lib,"Mswsock.lib")

using namespace MyThreadPool;
using namespace std;

#define MAX_NUM 64

class CTCPNet : public CINet
{
public:
	CTCPNet(CIMediator * pme);
	~CTCPNet();
public:
	bool Initial();
	void UnInitial();
	bool Send(sockaddr_in * addr,char * szbuf,int len);
public:
	bool PostAccept();
	bool PostRecv(MYSOCKETEX * pmysocket);
public:
	HANDLE m_ComIoPort;
	list<MYSOCKETEX *> m_lstSocket;
	CMyThreadPool m_threadpool;
public:
	SOCKET m_socket;
	CIMediator * m_pme;
};