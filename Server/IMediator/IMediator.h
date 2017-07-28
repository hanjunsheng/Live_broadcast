#pragma once
#include <WinSock2.h>
#include "packdef.h"
using namespace std;

class  CIMediator
{
public:
	CIMediator();
	virtual ~ CIMediator();
public:
	virtual bool Open() = 0;
	virtual void Close() = 0;
	virtual bool SendData(sockaddr_in * addr,char *szbuf,int len) = 0;
	virtual bool DealData(sockaddr_in *addr,char *szbuf) = 0;
	virtual bool SendTCPData(char *szbuf,int len) = 0;
	virtual bool DealTCPData(MYSOCKETEX * pmysock,char *szbuf) = 0;
};

