#pragma once

class  CIMediator
{
public:
	CIMediator();
	virtual ~ CIMediator();
public:
	virtual void SetServerAddr(long IP) = 0;
	virtual bool Open(HWND hwnd) = 0;
	virtual void Close() = 0;
	virtual bool SendData(char *szbuf,int len) = 0;
	virtual bool DealData(char *szbuf) = 0;
};

